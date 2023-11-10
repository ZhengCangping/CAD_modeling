#include"SolidProcess.h"

//函数实现
Solid* SolidProcess::SolidMultiRing()
{
	//定义一个如同8字形的模型的顶点信息
	//外环的点坐标
	glm::vec3 points_up_out[] = 
	{   
		glm::vec3(-1.0, -1.0, 0.0 ),
		glm::vec3(1.0, -1.0, 0.0 ),
		glm::vec3(1.0, 1.0, 0.0 ),
		glm::vec3(-1.0, 1.0, 0.0 )
	};

	//内环1的点坐标
	glm::vec3 points_up_in_1[] = 
	{
		glm::vec3(-0.8, -0.8, 0.0 ),
		glm::vec3(-0.1, -0.8, 0.0 ),
		glm::vec3(-0.1, -0.1, 0.0 ),
		glm::vec3(-0.8, -0.1, 0.0 ) 
	};

	//内环2的点坐标
	glm::vec3 points_up_in_2[] = 
	{ 
		glm::vec3(0.1,-0.8,0.0),
		glm::vec3(0.8,-0.8,0.0),
		glm::vec3(0.8,-0.1,0.0),
		glm::vec3(0.1,-0.1,0.0)
	};

	//内环3的点坐标
	glm::vec3 points_up_in_3[] =
	{
		glm::vec3(-0.8,0.1,0.0),
		glm::vec3(0.8,0.1,0.0),
		glm::vec3(0.8,0.8,0.0),
		glm::vec3(-0.8,0.8,0.0)
	};
	
	//欧拉操作
	// up
	Solid* solid_up = mvfs(points_up_out[0]);
	Loop* assistLp = solid_up->sfaces->floops;
	Loop* outloop = assistLp;

	// 构建外环
	mev(assistLp, points_up_out[0], points_up_out[1]);
	mev(assistLp, points_up_out[1], points_up_out[2]);
	mev(assistLp, points_up_out[2], points_up_out[3]);
	mef(assistLp, points_up_out[3], points_up_out[0]);

	// 构建内环1
	assistLp = solid_up->sfaces->nextf->floops;
	Loop* myLp = assistLp;
	mev(assistLp, points_up_out[0], points_up_in_1[0]);		//创建points_up_in_1[0]点，与points_up_out[0]相连
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_1[0]);		//删去上述边，剩下孤立的points_up_out[0]点
	mev(assistLp, points_up_in_1[0], points_up_in_1[1]);
	mev(assistLp, points_up_in_1[1], points_up_in_1[2]);
	mev(assistLp, points_up_in_1[2], points_up_in_1[3]);
	Loop* inloop = mef(assistLp, points_up_in_1[3], points_up_in_1[0]);
	kfmrh(inloop, outloop);		//打孔

	// 构建内环2
	assistLp = myLp;
	mev(assistLp, points_up_out[0], points_up_in_2[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_2[0]);
	mev(assistLp, points_up_in_2[0], points_up_in_2[1]);
	mev(assistLp, points_up_in_2[1], points_up_in_2[2]);
	mev(assistLp, points_up_in_2[2], points_up_in_2[3]);
	inloop = mef(assistLp, points_up_in_2[3], points_up_in_2[0]);
	kfmrh(inloop, outloop);

	// 构建内环3
	assistLp = myLp;
	mev(assistLp, points_up_out[0], points_up_in_3[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_3[0]);
	mev(assistLp, points_up_in_3[0], points_up_in_3[1]);
	mev(assistLp, points_up_in_3[1], points_up_in_3[2]);
	mev(assistLp, points_up_in_3[2], points_up_in_3[3]);
	inloop = mef(assistLp, points_up_in_3[3], points_up_in_3[0]);
	kfmrh(inloop, outloop);

	ListSolid(solid_up);

	glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.4f);	// 扫成距离

	//solid_up->sfaces->nextf = NULL;	//debug：这里不应该直接等于null，但是无法
	ListSolid(solid_up);
	Sweep(solid_up->sfaces, move_vec);

	return solid_up;
}

