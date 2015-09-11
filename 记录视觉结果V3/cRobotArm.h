#ifndef _COM2MOTION_H_
#define _COM2MOTION_H_

#include "stdio.h"
#include <iostream>
#include <io.h> //_access

#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class cRobotArm
{
public:
	bool ComInit();
	bool HitBall(double X_hit,double Y_hit,double Z_hit,double hit_vx0,double hit_vy0,double hit_vz0,double t_predict);
};

#endif