#include"SolidProcess.h"

//����ʵ��
Solid* SolidProcess::SolidMultiRing()
{
	//����һ����ͬ8���ε�ģ�͵Ķ�����Ϣ
	//�⻷�ĵ�����
	glm::vec3 points_up_out[] = 
	{   
		glm::vec3(-1.0, -1.0, 0.0 ),
		glm::vec3(1.0, -1.0, 0.0 ),
		glm::vec3(1.0, 1.0, 0.0 ),
		glm::vec3(-1.0, 1.0, 0.0 )
	};

	//�ڻ�1�ĵ�����
	glm::vec3 points_up_in_1[] = 
	{
		glm::vec3(-0.8, -0.8, 0.0 ),
		glm::vec3(-0.1, -0.8, 0.0 ),
		glm::vec3(-0.1, -0.1, 0.0 ),
		glm::vec3(-0.8, -0.1, 0.0 ) 
	};

	//�ڻ�2�ĵ�����
	glm::vec3 points_up_in_2[] = 
	{ 
		glm::vec3(0.1,-0.8,0.0),
		glm::vec3(0.8,-0.8,0.0),
		glm::vec3(0.8,-0.1,0.0),
		glm::vec3(0.1,-0.1,0.0)
	};

	//�ڻ�3�ĵ�����
	glm::vec3 points_up_in_3[] =
	{
		glm::vec3(-0.8,0.1,0.0),
		glm::vec3(0.8,0.1,0.0),
		glm::vec3(0.8,0.8,0.0),
		glm::vec3(-0.8,0.8,0.0)
	};
	
	//ŷ������
	// up
	Solid* solid_up = mvfs(points_up_out[0]);
	Loop* assistLp = solid_up->sfaces->floops;
	Loop* outloop = assistLp;

	// �����⻷
	mev(assistLp, points_up_out[0], points_up_out[1]);
	mev(assistLp, points_up_out[1], points_up_out[2]);
	mev(assistLp, points_up_out[2], points_up_out[3]);
	mef(assistLp, points_up_out[3], points_up_out[0]);

	// �����ڻ�1
	assistLp = solid_up->sfaces->nextf->floops;
	Loop* myLp = assistLp;
	mev(assistLp, points_up_out[0], points_up_in_1[0]);		//����points_up_in_1[0]�㣬��points_up_out[0]����
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_1[0]);		//ɾȥ�����ߣ�ʣ�¹�����points_up_out[0]��
	mev(assistLp, points_up_in_1[0], points_up_in_1[1]);
	mev(assistLp, points_up_in_1[1], points_up_in_1[2]);
	mev(assistLp, points_up_in_1[2], points_up_in_1[3]);
	Loop* inloop = mef(assistLp, points_up_in_1[3], points_up_in_1[0]);
	kfmrh(inloop, outloop);		//���

	// �����ڻ�2
	assistLp = myLp;
	mev(assistLp, points_up_out[0], points_up_in_2[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_2[0]);
	mev(assistLp, points_up_in_2[0], points_up_in_2[1]);
	mev(assistLp, points_up_in_2[1], points_up_in_2[2]);
	mev(assistLp, points_up_in_2[2], points_up_in_2[3]);
	inloop = mef(assistLp, points_up_in_2[3], points_up_in_2[0]);
	kfmrh(inloop, outloop);

	// �����ڻ�3
	assistLp = myLp;
	mev(assistLp, points_up_out[0], points_up_in_3[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_3[0]);
	mev(assistLp, points_up_in_3[0], points_up_in_3[1]);
	mev(assistLp, points_up_in_3[1], points_up_in_3[2]);
	mev(assistLp, points_up_in_3[2], points_up_in_3[3]);
	inloop = mef(assistLp, points_up_in_3[3], points_up_in_3[0]);
	kfmrh(inloop, outloop);

	ListSolid(solid_up);

	glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.4f);	// ɨ�ɾ���

	//solid_up->sfaces->nextf = NULL;	//debug�����ﲻӦ��ֱ�ӵ���null�������޷�
	ListSolid(solid_up);
	Sweep(solid_up->sfaces, move_vec);

	return solid_up;
}

