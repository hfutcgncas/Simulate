

#ifndef _CAM3D_H_
#define _CAM3D_H_

#include "camer.h"
#include "GlobleSrouce.h"


class cHis3DTemp
{
public:	
	vector<double> tempX;
	vector<double> tempY;
	vector<double> tempZ;
	vector<double> tempVx;
	vector<double> tempVy;
	vector<double> tempVz;
	vector<double> tempTime;

	void push_back(double x,double y,double z,double vx,double vy,double vz,double time);
	void clear();
	int  size();
	void  del();//删除队列头部的数据
	void copy(cHis3DTemp src);
};

struct sBallStatue
{
	bool   direction;
	double x_last;
	double t_last;
	bool   predicted;
};

struct sBallinf
{
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	double time;
};

class cCam3D
{
private:
	cCamer CamA;
	cCamer CamB;
	cCamer CamC;
	cCamer CamD;

	cTempData CamTempA,CamTempB;
	int ContA,ContB,ContC,ContD;

	

public:
	//sBallStatue BallStatue,BallStatue_Last,BallStatue_doubt_Last;
	//cHis3DTemp ballPosHis,ballDoubtPosHis;

	bool cCam3D::Sync();
	//bool Init(char* IP_A,u_short port_A,char* IP_B,u_short port_B);//初始化3D摄像机的参数，以及与两台高速摄像机建立连接
	bool cCam3D::Init(char* IP_A, u_short port_A,  //初始化3D摄像机的参数，以及与两台高速摄像机建立连接
		char* IP_B, u_short port_B,
		char* IP_C, u_short port_C,
		char* IP_D, u_short port_D);
	char RefreshSrcData(void);//刷新高速摄像机的结果 
	bool ReConstruction(char rt,double* pt,double* px,double* py,double* pz,double* pvx,double* pvy,double* pvz);
	
};


//=======================================

void InitMatrax();

Matrix * Cam3D(Matrix* MA, cTempData FilterTempA,Matrix* MB,cTempData FilterTempB);
Matrix * Cam3D(Matrix* MA, cTempData FilterTempA,Matrix* MB,cTempData FilterTempB,Matrix* MC, cTempData FilterTempC);

//==================================
double predictX(double x0,double y0,double z0,double Vx0,double Vy0,double Vz0,
			   double *X_land,double * Y_land,double * Z_land,
			   double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
			   double *X_hit,double * Y_hit,double *Z_hit,
			   double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane);

inline double signFun(double a);


double Predict_trace(cHis3DTemp ballPosHis,
			   double *X_land,double * Y_land,double * Z_land,
			   double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
			   double *X_hit,double * Y_hit,double *Z_hit,
			   double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane);


void predict_TR(double x0,double y0,double z0,double Vx0,double Vy0,double Vz0,
	double & X_t,double & Y_t,double & Z_t,double &Vx_t,double &Vy_t,double &Vz_t,double &t_t,double x_target);


/*double Predict_Model(   double x0, double y0, double z0, double Vx0, double Vy0, double Vz0, 
						double *X_land,double *Y_land,double *Z_land,double *land_Time,
						double *land_vx0,double *land_vy0,double *land_vz0, double *land_vxr,double *land_vyr,double *land_vzr, 
						double land_wx0,double land_wy0,double land_wz0, double *land_wxr,double *land_wyr,double *land_wzr,						
						double *X_hit, double *Y_hit, double *Z_hit, double *hit_vx0, double *hit_vy0, double *hit_vz0,
						double stopPlane);*/
double Predict_Model(  double *pballStart, 
	double *X_land,double *Y_land,double *Z_land,double *land_Time,
	double *land_vx0,double *land_vy0,double *land_vz0, double *land_vxr,double *land_vyr,double *land_vzr, 
	double land_wx0,double land_wy0,double land_wz0, double *land_wxr,double *land_wyr,double *land_wzr,						
	double *X_hit, double *Y_hit, double *Z_hit, double *hit_vx0, double *hit_vy0, double *hit_vz0,
	double stopPlane);

double Predict_traceV2(cHis3DTemp ballPosHis,double* ballHit);
/*
double Predict_traceV2(cHis3DTemp ballPosHis,
	double *X_land,double * Y_land,double * Z_land,
	double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
	double *X_hit,double * Y_hit,double *Z_hit,
	double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane);*/
//==================================

#endif