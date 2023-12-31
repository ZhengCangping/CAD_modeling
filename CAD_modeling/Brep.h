#ifndef BREP
#define BREP

#include<cstdlib>
#include<iostream>
#include<glm/glm.hpp>

using namespace std;

// 半边数据结构定义

class Solid;
class Face;
class Loop;
class HalfEdge;
class Vertex;
class Edge;

static int out = 0;

// 实体类
class Solid
{
public:
	
	int solidno;	// 实体编号				
	static int total_solidnum;		// 实体总数

	Solid* nexts;	// 指向下一个实体的指针
	Solid* prevs;	// 指向上一个实体的指针

	Vertex* sverts; // 实体指向其上顶点链表的指针
	Edge* sedges;   // 实体指向其上物理边链表的指针
	Face* sfaces;   // 实体指向其上面链表的指针
	//Loop* sloops;  // 实体指向其上环链表的指针

	
	int numvertex;  // 实体的顶点数
	int numedge;    // 实体的边数
	int numface;	// 实体的面数
	//int numloop;   // 实体的环数

	
	Solid();		// 构造函数
	virtual ~Solid();	//析构函数（声明为虚函数能保证安全）

	Vertex* findVertex(glm::vec3 pos);		// 查找点

	bool deleteVertex(Vertex* v);			// 删去点
	bool deleteEdge(Edge* eg);				// 删去物理边
	bool deleteFace(Face* face);			// 删去面
	//bool deleteLoop(Loop* lp);				// 删去环

	bool addVertexToSolid(Vertex* v);		// 添加顶点
	bool addEdgeToSolid(Edge* eg);			// 添加物理边
	bool addFaceToSolid(Face* face);		// 添加面
	// bool addLoopToSolid(Loop* lp);			// 添加环

};

class Face
{
public:

	int faceno;					// 编号
	static int total_facenum;	// 总共有几个面

	Solid* fsolid; // 面所在的实体
	Loop* floops;  // 面上所有loop组成的链表的头节点
	Face* nextf;   // 下一个面
	Face* prevf;   // 前一个面


	Face();		
	~Face();	

	int Size();			//查询face中loop的个数
	void setInnerLoop();	//设置内环
	Loop* GetOuterLoop();	//得到第一个外环
	bool addLoopToFace(Loop* lp);	// 添加loop

};

class Loop
{
public:

	int loopno;					// loop的序号
	static int total_loopnum;	// 总共生成了几个环

	Face* lface; // loop所在的面
	HalfEdge* lhedge; //  loop中半边组成的链表的头节点
	Loop* nextl;	// 下一个环
	Loop* prevl;	// 上一个环

	bool is_inner_loop;	//标记是否为内环


	Loop();	
	~Loop(); 

	bool addHalfEdgeToLoop(HalfEdge* he);	// 添加半边，如果添加成功，返回true
	bool isVertexInLoop(Vertex* v);			// 判断vertex是否在loop中
	HalfEdge* findHalfEdge(Vertex* v, HalfEdge* he = NULL);	// 从he开始寻找以v为起点的半边 

};

class Edge
{
public:
	
	int edgeno;					// edge序号
	static int total_edgenum;	// 总共生成了几条物理边

	HalfEdge* he1; // 物理边的半边1
	HalfEdge* he2; // 物理边的半边2
	Edge* nexte;   // 下一条物理边
	Edge* preve;   // 上一条物理边

	
	Edge();		
	~Edge();    

};

class HalfEdge
{
public:

	int halfedgeno;					// 半边序号
	static int total_halfegnum;		// 总共生成了几条半边

	Edge* edg;		// 半边所在的物理边
	Vertex* hestartv;	// 半边的起点
	Loop* wloop;    // 半边所在的loop
	HalfEdge* adjacent; // 指向同一物理边的半边
	HalfEdge* next;		// 下一条半边
	HalfEdge* prev;		// 上一条半边


	HalfEdge();			
	~HalfEdge();		


};

class Vertex
{
public:

	int vertexno;					// vertex 序号
	static int total_vertexnum;		// 总共生成了几个vertex

	Vertex* nextv;	// 下一个vertex
	Vertex* prevv;  // 上一个vertex
	glm::vec3 pos;	//三维空间位置

	Vertex();
	Vertex(glm::vec3 pos);	
};


#endif // !BREP

