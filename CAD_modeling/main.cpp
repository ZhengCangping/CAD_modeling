#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include"SolidProcess.h"
#include"Shader.h"
#include"Renderer.h"

//回调函数-原型：每次改变窗口大小的时候调用，用于改变视口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);
//窗口尺寸
float SCR_WIDTH = 800, SCR_HEIGHT = 600;

int item = 0;	//是输出着色模型，还是线框模型，0--着色模型，1--线框模型

Solid* solid;	//建立的实体

int main()
{
	glfwInit();	//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		//告诉GLFW使用的OpenGL的版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CAD_Modeling", NULL, NULL);
	if (window == NULL) {	//判断窗口是否创建成功
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	//通知GLFW把窗口的上下文设置为当前线程的主上下文

	//注册回调函数---改变窗口大小时改变视口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//定义实体模型
	SolidProcess* sweepObj = new SolidProcess();
	solid = sweepObj->SolidMultiRing(); //根据定义信息构造实体模型
	Euler::ListSolid(solid); //在控制台输出实体信息

	float solidLines[1000]; //依次存储物理边的两半边的起点信息
	int cnt = 0; //line模式下存储的元素个数
	Edge* eg = solid->sedges; //获取存储了实体上所有边的链表
	HalfEdge* temphe;
	//这个循环条件写得很不常见，但其实目的就是遍历链表且检查元素是否非空
	for (; eg; eg = eg->nexte) {
		//获取半边1的起点坐标，依次存入数组
		temphe = eg->he1;
		Vertex* p = temphe->hestartv;
		solidLines[cnt++] = p->pos.x;
		solidLines[cnt++] = p->pos.y;
		solidLines[cnt++] = p->pos.z;
		//获取半边2的起点坐标，依次存入数组
		temphe = eg->he2;
		p = temphe->hestartv;
		solidLines[cnt++] = p->pos.x;
		solidLines[cnt++] = p->pos.y;
		solidLines[cnt++] = p->pos.z;
	}
	std::cout << "vertices中的点的个数 cnt = " << cnt << std::endl;

	for (int i = 0; i < cnt; i++) {
		if (i > 0 && i % 6 == 0) std::cout << std::endl;
		std::cout << solidLines[i] << " \t";
		//solidVertices[i] /= 200;
	}

	//解析着色模式下的顶点信息
	float solidTriangles[1000];
	int num = 0;
	Renderer ren = Renderer(solid);
	ren.GenerateTris();     //将solid的每个面三角化
	for (auto tri : ren.triangles) {
		for (int i = 0; i < 3; i++)
		{
			solidTriangles[num++] = tri.v[i].x;
			solidTriangles[num++] = tri.v[i].y;
			solidTriangles[num++] = tri.v[i].z;
		}
	}
	std::cout <<std::endl<< "triangles中的点的个数 num = " << num << std::endl;
	for (int i = 0; i < num; i++) {
		if (i > 0 && i % 9 == 0) std::cout << std::endl;
		std::cout << solidTriangles[i] << " \t";
		//solidTriangles[i] /= 150;
	}


	

	//创建两个VAO,两个VBO，用两个shader,0为triangle，1为line
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

	//为2个模式各设置一个着色器
	Shader ourShader[2];
	ourShader[0]=Shader("./ShaderFile/shader.vs", "./ShaderFile/shader.fs");	//各点颜色直接定义在vertex shader里了
	ourShader[1] = Shader("./ShaderFile/shader.vs", "./ShaderFile/shader.fs");
	//ourShader_Line.use();

	glEnable(GL_DEPTH_TEST);	//启用深度测试

	//渲染循环，会不断检查窗口是否被关闭
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);	//检查输入
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//设置清屏的颜色
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);	//清空屏幕

		ourShader[item].use();

		//初始MVP矩阵
		glm::mat4 model = glm::mat4(1.0f);		
		glm::mat4 view = glm::mat4(1.0f);		
		glm::mat4 projection = glm::mat4(1.0f);	

		//MVP矩阵赋值
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		//给shader设置mvp矩阵
		ourShader[item].setMat4("model", model);
		ourShader[item].setMat4("view", view);
		ourShader[item].setMat4("projection", projection);

		glBindVertexArray(VAO[item]);
		if (item == 0)
			glDrawArrays(GL_TRIANGLES, 0, num / 3);
		else glDrawArrays(GL_LINES, 0, cnt / 3);



		glfwSwapBuffers(window);	//这里采用双缓冲，交换颜色缓冲
		glfwPollEvents();	//检查是否触发事件，若触发则调用相应回调函数
	}

	//释放所有资源
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
	//检查是否按下了ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//检查是否按下了空格键
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		item = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		item = 1;
}