#pragma once
#include <vector>


using std::vector;

class cImgCoordinateStruct
{
public:
	cImgCoordinateStruct();
	~cImgCoordinateStruct();

	vector<double> u;
	vector<double> v;
	vector<double> t;

	void push_back(double x, double y, double time);
	void clear();
	int  size();
	void  del();
	void copy(cImgCoordinateStruct src);

	void disp();
};

