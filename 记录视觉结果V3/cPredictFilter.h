
/**************************************************************************
Copyright:		JR.Liu
Author:			刘健冉
Date:			2015-9-9
Description:	矩阵化的球轨迹预测
platform:       VS2013
Bug:            不能处理过大的数
**************************************************************************/
#pragma once
#include <iostream>  
#include <Eigen/Dense>  


using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;

using namespace std;

class cPredictFilter
{
public:
	cPredictFilter();
	~cPredictFilter();

private:
    double Tc;
	double g;
	double Kd;
	double Km;

	double aa;
	double et;

	double t_last;
	bool isInited;

public:
	VectorXf S; //球的状态，[x，y, z, vx, vy, vz]
	VectorXf X;
	Vector3f V;
	Vector3f W;

	MatrixXf A; //转移矩阵，随时改变
	VectorXf B;
	MatrixXf A1;

	MatrixXf q;  //一次递推导致的系统误差协方差阵
	MatrixXf P;
	MatrixXf R;


	Matrix3f accelerMatrix;


	VectorXf predict_OneStep(VectorXf S_in);
public:
	VectorXf predict_DT(VectorXf S_in, double dT, MatrixXf* pA_DT, MatrixXf* pQ);
	VectorXf predict_hit(VectorXf S_in, double x_hit);
	bool filterA(VectorXf S_in, double t_in);
};

