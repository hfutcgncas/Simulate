
/**************************************************************************
Copyright:		JR.Liu
Author:			����Ƚ
Date:			2015-9-9
Description:	���󻯵���켣Ԥ��
platform:       VS2013
Bug:            ���ܴ���������
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
	VectorXf S; //���״̬��[x��y, z, vx, vy, vz]
	VectorXf X;
	Vector3f V;
	Vector3f W;

	MatrixXf A; //ת�ƾ�����ʱ�ı�
	VectorXf B;
	MatrixXf A1;

	MatrixXf q;  //һ�ε��Ƶ��µ�ϵͳ���Э������
	MatrixXf P;
	MatrixXf R;


	Matrix3f accelerMatrix;


	VectorXf predict_OneStep(VectorXf S_in);
public:
	VectorXf predict_DT(VectorXf S_in, double dT, MatrixXf* pA_DT, MatrixXf* pQ);
	VectorXf predict_hit(VectorXf S_in, double x_hit);
	bool filterA(VectorXf S_in, double t_in);
};

