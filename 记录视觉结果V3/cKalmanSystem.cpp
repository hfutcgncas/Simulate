
/**************************************************************************
Copyright:		JR.Liu
Author:			刘健冉
Date:			2015-9-08
Description:	kalman滤波的C++实现
Bug:            不能处理过大的数
**************************************************************************/
#include "cKalmanSystem.h"


//using Eigen::MatrixXd;  


cKalmanSystem::cKalmanSystem()
{
}


cKalmanSystem::~cKalmanSystem()
{
}


/*************************************************
Function:				cSystemInform::checkSystem
Description:			检测系统内各参数维度是否正确
Calls:                  cSystemInform::InitSystem,
Table Accessed:
Table Updated:
Input:
Output:
Return:					是否初始化成功
Others:					不导出
*************************************************/
bool cKalmanSystem::checkSystem()
{
	if (A.rows() != x_Dim || A.cols() != x_Dim){ cout << "error NO:"<<1 << endl; return false; }
	if (B.rows() != x_Dim || B.cols() != u_Dim){ cout << "error NO:" << 2 << endl; return false; }
	if (H.rows() != z_Dim || H.cols() != x_Dim){ cout << "error NO:" << 3 << endl; return false; }
	if (Q.rows() != x_Dim || Q.cols() != x_Dim){ cout << "error NO:" << 4 << endl; return false; }
	if (P.rows() != x_Dim || P.cols() != x_Dim){ cout << "error NO:" << 5 << endl; return false; }
	if (R.rows() != z_Dim || R.cols() != z_Dim){ cout << "error NO:" << 6 << endl; return false; }
	return true;
}


	/*************************************************
	Function:				cSystemInform::InitSystem
	Description:			初始化系统信息
	Calls:
	Table Accessed:
	Table Updated:
	Input:					各个参数的信息: a 转移矩阵； b 偏移矩阵 ； h 观测矩阵 ； q 模型方差； p 累积结果误差方差； r 观测误差方差
	Output:
	Return:					是否初始化成功
	Others:					不导出
	*************************************************/
bool cKalmanSystem::InitSystem(MatrixXd a, MatrixXd b, MatrixXd h, MatrixXd q, MatrixXd p, MatrixXd r, VectorXd x_init, VectorXd u_init)
{
		A = a;
		B = b;
		H = h;
		Q = q;
		P = p;
		R = r;

		x = x_init;
		u = u_init;

		x_Dim = x.rows();
		z_Dim = h.rows();
		u_Dim = u.rows();


		return checkSystem();
}


	/*************************************************
	Function:				cSystemInform::upDateState
	Description:			根据新的观测数据更新系统状态，kalman滤波的核心公式
	Calls:
	Table Accessed:
	Table Updated:
	Input:					z_new; 新的观测值
	Output:					isSucces (bool)
	Return:					是否初始化成功
	Others:					不导出
	*************************************************/
bool cKalmanSystem::upDateState(VectorXd z_new)
{
		if (z_new.rows() != z_Dim)
		{
			cout << "wrong Observe State Z" << endl;
			return false;
		}
		z = z_new;



		//~~~~~~~~~~~~~~~~~~~~~~~
		// Predict
		//   Predicted state estimate
		MatrixXd x_pre = A*x + B*u;
		//   Predicted covariance estimate
		MatrixXd P_pre = A*P*A.transpose() + Q;

		//~~~~~~~~~~~~~~~~~~~~~~~

		// Update
		//Innovation or measurement residual
		MatrixXd y = z - H * x_pre;
		//Innovation(or residual) covariance
		MatrixXd S = H * P * H.transpose() + R;
		//Near - optimal Kalman gain
		K = P * H.transpose() * S.inverse();
		//Updated covariance estimate
		P = P_pre - K * H * P_pre;
		//Updated state estimate
		x = x_pre + K * y;


		return true;
}