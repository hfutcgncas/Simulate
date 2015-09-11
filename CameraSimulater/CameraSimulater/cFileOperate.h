#pragma once
#include <iostream>  
#include <iomanip>  
#include <fstream>  
using namespace std;
class cFileOperate
{
public:
	cFileOperate(string txtpath);
	~cFileOperate();

	ifstream myfile;

	char buffer[100];
	float data_Read[500][4];

	int LoadFullFile();


};



