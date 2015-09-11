#ifndef _GLOBLESROUCE_H_
#define _GLOBLESROUCE_H_

#include "time.h" // time
#include "stdio.h"

#include <io.h> //_access
#include"string.h"
#include "direct.h" // mkdir
#include "time.h" // time

#include "conio.h"//键盘




#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "Cam3D.h"

#define PREDICT_BACKFORWARD -1
#define PREDICT_NORMAL 1
#define PREDICT_NOREBOND 2
#define PREDICT_TIMEOUT 3
#define PREDICT_TOOSHORT 4
//时钟================================
class cClock
{
public:
	LARGE_INTEGER litmp;
	LONGLONG timeSysStart,timeSysNow;
	double dfFreq,dfMinus; 

	cClock();
	double now();
	void reset();
};

//记录文件===========================
/*
class DataFile
{
public:
	char fileName[100];
	FILE *fp88;
	bool bFileCreate88 = false;
	int fileNameIndex88=0;
	bool m_bStartSave=false;
	char DirName[100];

};*/
//=================================================================================================
//=================================================================================================
//=================================================================================================
//shell===========================
class cShell
{
public:
	char key;

	char getCommand();
};


//=================================================================================================
//=================================================================================================
//=================================================================================================


class cFile
{
	char	fileName[100];
	FILE	*fp;
	bool	bFileCreate  ;
	int		fileNameIndex;
	char	DirName[100];
public:
	cFile();

	bool m_bStartSave;
	bool FileInit();
	void SaveTraceData(double x,double y,double z,double t,double vx,double vy,double vz,int flag);
};
//=================================================================================================
//=================================================================================================
//=================================================================================================
/* x,y,z,Vx,Vy,Vz,Wx,Wy,Wz,t */
class BallPoint
{
public:
	double x;
	double y;
	double z;

	double Vx;
	double Vy;
	double Vz;

	double Wx;
	double Wy;
	double Wz;

	double t;

	void Copy(BallPoint src);

	BallPoint operator*(double fac);


	BallPoint operator+(BallPoint &A);
	BallPoint operator-(BallPoint &A);

	void DispPoint();
};
//=================================================================================================
//=================================================================================================
//=================================================================================================
class PrediectGroup
{
public:
	BallPoint Ball_Start;
	BallPoint Ball_Stop;
	double Confidence;

};
//=================================================================================================
//=================================================================================================
//=================================================================================================
static class cBallModel
{
	inline double signFun(double a)
	{
		if (a == 0)
		{
			return 0.0;
		}
		if (a> 0)
		{
			return a;
		}
		else
			return -a;
	}
public:
	bool predict_OneStep(BallPoint In, BallPoint* pOut, double dt);
	bool predict_Rebound(BallPoint In, BallPoint* pOut);
	
	int predict(BallPoint startPoint, BallPoint* pendPoint, double* confidence, double stopPlane);
	int predict_dt(BallPoint startPoint, BallPoint* pEndPoint, double* confidence, double dt);
	
};
//=================================================================================================
//=================================================================================================
//=================================================================================================
class cBallPosTemp
{
public:
	vector<double> tempX;
	vector<double> tempY;
	vector<double> tempZ;
	vector<double> tempVx;
	vector<double> tempVy;
	vector<double> tempVz;
	vector<double> tempWx;
	vector<double> tempWy;
	vector<double> tempWz;
	vector<double> tempTime;

	void push_back(double x, double y, double z, double vx, double vy, double vz, double wx, double wy, double wz, double time);
	void push_back(BallPoint newPoint);

	void clear();
	int  size();
	void  del();//删除队列头部的数据
	void copy(cBallPosTemp src);
};
//=================================================================================================
//=================================================================================================
//=================================================================================================
class cPrediectOutput
{
public:
	BallPoint Output;
	BallPoint EstimatedBallStatue;
	cBallPosTemp BallTemp;
	cPrediectOutput();

	PrediectGroup HisGroup;

	void cPrediectOutput::RefreashOutput(PrediectGroup CurrentGroup);

	
};





#endif //_GLOBLESROUCE_H_