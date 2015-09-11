#include "cClock.h"



cClock::~cClock()
{
}


cClock::cClock()
{
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

void cClock::reset()
{
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

double cClock::now()//单位为ms
{
	double nowRead;
	QueryPerformanceCounter(&litmp);
	timeSysNow = litmp.QuadPart;
	timeSysNow -= timeSysStart;
	if (timeSysNow<0)timeSysNow += 0x7FFFFFFF;  //LONGLONG 为64位整形，故如此进位
	nowRead = timeSysNow / dfFreq*1000.0 ;
	if (nowRead>1000000)
	{
		nowRead -= 1000000;
		this->reset();
	}
	return nowRead + bias;
}

void cClock::SetNow(double setTime)
{
	bias = setTime;
}