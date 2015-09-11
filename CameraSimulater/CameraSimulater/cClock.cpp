#include "cClock.h"



cClock::~cClock()
{
}


cClock::cClock()
{
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

void cClock::reset()
{
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

double cClock::now()//��λΪms
{
	double nowRead;
	QueryPerformanceCounter(&litmp);
	timeSysNow = litmp.QuadPart;
	timeSysNow -= timeSysStart;
	if (timeSysNow<0)timeSysNow += 0x7FFFFFFF;  //LONGLONG Ϊ64λ���Σ�����˽�λ
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