#ifndef SOLIDPROCESS
#define SOLIDPROCESS

#include"Euler.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>

using namespace std;

class SolidProcess : public Euler
{
public:

											
	Solid* SolidMultiRing();  //函数原型，定义在同名的.cpp文件中
							

};




#endif // !SOLIDPROCESS
