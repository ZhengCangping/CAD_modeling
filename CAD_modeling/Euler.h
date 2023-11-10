#ifndef EULEROPERATIONS
#define EULEROPERATIONS

#include "Brep.h"
//#include <json\json.h>

//using namespace Json;
using namespace std;

class Euler
{
protected:
	Solid* mvfs(glm::vec3 pos);										// 创造点/面/体 make vertex face solid
	HalfEdge* mev(Loop* lp, glm::vec3 oldPos, glm::vec3 newPos);	// 创造边连接旧点与新点 make edge vertex
	Loop* mef(Loop* lp, glm::vec3 pos1, glm::vec3 pos2);			// 创造边连接两点，构成一个新面 make edge face
	Loop* kemr(Loop* lp, glm::vec3 pos1, glm::vec3 pos2);			// 消去环中的一条边（物理边），生成一个内环 kill edge, make ring
	bool kfmrh(Loop* innerLoop, Loop* outerLoop);					// 删除一个面并将其定义为另一个面的内环 kill face make ring and hole

	//bool kvfs(Solid* s, double* pcoord);						// kill vertex face solid
	//bool kev(Solid* s, double* coord1, double* coord2);			// kill edge vertex
	//bool kef(Loop* lp, double* coord1, double* coord2);			// kill edge face
	//HalfEdge* mekr(Loop* lp, Loop* todel, double* coord1, double* coord2); //make edge kill ring
	//Face* mfkrh(Loop* outerLoop, Loop* innerLoop);				// make face kill ring and hole

	void Sweep(Face* f, glm::vec3 move_vec);

public:
	static void ListSolid(Solid* s);									// Output Solid Hierarchy For Debug

};



#endif // !EULEROPERATIONS

