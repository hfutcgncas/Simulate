#pragma once
#include "time.h" // time
#include "stdio.h"

#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class cClock
{
public:
	cClock();
	~cClock();

	// ±÷”================================

	LARGE_INTEGER litmp;
	LONGLONG timeSysStart, timeSysNow;
	double dfFreq, dfMinus;

	double bias;

	double now();
	void reset();

	void SetNow(double setTime);
};



