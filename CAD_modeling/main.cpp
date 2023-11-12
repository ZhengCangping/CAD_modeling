#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include"SolidProcess.h"
#include"Shader.h"
#include"Renderer.h"

//�ص�����-ԭ�ͣ�ÿ�θı䴰�ڴ�С��ʱ����ã����ڸı��ӿڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//��������
void processInput(GLFWwindow* window);
//���ڳߴ�
float SCR_WIDTH = 800, SCR_HEIGHT = 600;

int item = 0;	//�������ɫģ�ͣ������߿�ģ�ͣ�0--��ɫģ�ͣ�1--�߿�ģ��

Solid* solid;	//������ʵ��

int main()
{
	glfwInit();	//��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//����GLFWʹ�õ�OpenGL�İ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//����һ�����ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CAD_Modeling", NULL, NULL);
	if (window == NULL) {	//�жϴ����Ƿ񴴽��ɹ�
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	//֪ͨGLFW�Ѵ��ڵ�����������Ϊ��ǰ�̵߳���������

	//ע��ص�����---�ı䴰�ڴ�Сʱ�ı��ӿڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����ʵ��ģ��
	SolidProcess* sweepObj = new SolidProcess();
	solid = sweepObj->SolidMultiRing(); //���ݶ�����Ϣ����ʵ��ģ��
	Euler::ListSolid(solid); //�ڿ���̨���ʵ����Ϣ

	float solidLines[1000]; //���δ洢����ߵ�����ߵ������Ϣ
	int cnt = 0; //lineģʽ�´洢��Ԫ�ظ���
	Edge* eg = solid->sedges; //��ȡ�洢��ʵ�������бߵ�����
	HalfEdge* temphe;
	//���ѭ������д�úܲ�����������ʵĿ�ľ��Ǳ��������Ҽ��Ԫ���Ƿ�ǿ�
	for (; eg; eg = eg->nexte) {
		//��ȡ���1��������꣬���δ�������
		temphe = eg->he1;
		Vertex* p = temphe->hestartv;
		solidLines[cnt++] = p->pos.x;
		solidLines[cnt++] = p->pos.y;
		solidLines[cnt++] = p->pos.z;
		//��ȡ���2��������꣬���δ�������
		temphe = eg->he2;
		p = temphe->hestartv;
		solidLines[cnt++] = p->pos.x;
		solidLines[cnt++] = p->pos.y;
		solidLines[cnt++] = p->pos.z;
	}
	std::cout << "vertices�еĵ�ĸ��� cnt = " << cnt << std::endl;

	for (int i = 0; i < cnt; i++) {
		if (i > 0 && i % 6 == 0) std::cout << std::endl;
		std::cout << solidLines[i] << " \t";
		//solidVertices[i] /= 200;
	}

	//������ɫģʽ�µĶ�����Ϣ
	float solidTriangles[1000];
	int num = 0;
	Renderer ren = Renderer(solid);
	ren.GenerateTris();     //��solid��ÿ�������ǻ�
	for (auto tri : ren.triangles) {
		for (int i = 0; i < 3; i++)
		{
			solidTriangles[num++] = tri.v[i].x;
			solidTriangles[num++] = tri.v[i].y;
			solidTriangles[num++] = tri.v[i].z;
		}
	}
	std::cout <<std::endl<< "triangles�еĵ�ĸ��� num = " << num << std::endl;
	for (int i = 0; i < num; i++) {
		if (i > 0 && i % 9 == 0) std::cout << std::endl;
		std::cout << solidTriangles[i] << " \t";
		//solidTriangles[i] /= 150;
	}


	

	//��������VAO,����VBO��������shader,0Ϊtriangle��1Ϊline
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidTriangles), solidTriangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(solidLines), solidLines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Ϊ2��ģʽ������һ����ɫ��
	Shader ourShader[2];
	ourShader[0]=Shader("./ShaderFile/shader.vs", "./ShaderFile/shader.fs");	//������ɫֱ�Ӷ�����vertex shader����
	ourShader[1] = Shader("./ShaderFile/shader.vs", "./ShaderFile/shader.fs");
	//ourShader_Line.use();

	glEnable(GL_DEPTH_TEST);	//������Ȳ���

	//��Ⱦѭ�����᲻�ϼ�鴰���Ƿ񱻹ر�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);	//�������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//������������ɫ
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);	//�����Ļ

		ourShader[item].use();

		//��ʼMVP����
		glm::mat4 model = glm::mat4(1.0f);		
		glm::mat4 view = glm::mat4(1.0f);		
		glm::mat4 projection = glm::mat4(1.0f);	

		//MVP����ֵ
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		//��shader����mvp����
		ourShader[item].setMat4("model", model);
		ourShader[item].setMat4("view", view);
		ourShader[item].setMat4("projection", projection);

		glBindVertexArray(VAO[item]);
		if (item == 0)
			glDrawArrays(GL_TRIANGLES, 0, num / 3);
		else glDrawArrays(GL_LINES, 0, cnt / 3);



		glfwSwapBuffers(window);	//�������˫���壬������ɫ����
		glfwPollEvents();	//����Ƿ񴥷��¼����������������Ӧ�ص�����
	}

	//�ͷ�������Դ
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);



	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//����Ƿ�����ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//����Ƿ����˿ո��
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		item = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		item = 1;
}