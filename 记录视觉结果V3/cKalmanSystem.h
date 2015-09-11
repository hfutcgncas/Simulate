/**************************************************************************
Copyright:		JR.Liu
Author:			刘健冉
Date:			2015-9-08
Description:	kalman滤波的C++实现 
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


class cKalmanSystem
{
public:
	cKalmanSystem();
	~cKalmanSystem();

private:
	int x_Dim;   //状态量的维度
	int z_Dim;	 //观测量的维度
	int u_Dim;
public:
	MatrixXd A;  // transition matrix 转移矩阵
	MatrixXd B;  // x     =  A x  +  B  u
	VectorXd x;  //  i+1        i        i+1	
	MatrixXd P;  //系统误差

	MatrixXd H;  // 观测矩阵
	MatrixXd Q;  // z     =  H x  
	VectorXd z;  //  i          i
	MatrixXd R;  //观测误差

	VectorXd u;  //系统输入或偏差
	MatrixXd K;  //增益

#pragma region functionDef  
private:
	/*************************************************
	Function:				cSystemInform::checkSystem
	Description:			检测系统内各参数维度是否正确
	*************************************************/
	bool checkSystem();
public:
	/*************************************************
	Function:				cSystemInform::InitSystem
	Description:			初始化系统信息
	*************************************************/
	bool InitSystem(MatrixXd a, MatrixXd b, MatrixXd h, MatrixXd q, MatrixXd p, MatrixXd r, VectorXd x_init, VectorXd u_init);
	/*************************************************
	Function:				cSystemInform::upDateState
	Description:			根据新的观测数据更新系统状态，kalman滤波的核心公式
	*************************************************/
	bool upDateState(VectorXd z_new);
#pragma endregion 


};
