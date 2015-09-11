#include "cImgCoordinateStruct.h"
#include "iostream"

using namespace std;

cImgCoordinateStruct::cImgCoordinateStruct()
{
}


cImgCoordinateStruct::~cImgCoordinateStruct()
{
}

//========================================================
//7、乒乓球图像坐标的队列实现
//========================================================
void cImgCoordinateStruct::push_back(double x, double y, double time)
{
	u.push_back(x);
	v.push_back(y);
	t.push_back(time);
}
void cImgCoordinateStruct::clear()
{
	u.clear();
	v.clear();
	t.clear();
}
int cImgCoordinateStruct::size()
{
	return (int)(u.size());
}
void cImgCoordinateStruct::del()
{
	u.erase(u.begin());
	v.erase(v.begin());
	t.erase(t.begin());
}
void cImgCoordinateStruct::copy(cImgCoordinateStruct src)
{
	u = src.u;
	v = src.v;
	t = src.t;
}

void cImgCoordinateStruct::disp()
{
	cout << "====================================" << endl;
	for (int i = 0; i < u.size(); i++)
	{
		cout << u[i] << " " << v[i] << " " << t[i] << endl;
	}
	cout << "====================================" << endl;
}