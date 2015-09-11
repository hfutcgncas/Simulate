/**************************************************************************
Copyright:		JR.Liu
Author:			刘健冉
Date:			2015-9-9
Description:	矩阵化的球轨迹预测
platform:       VS2013
Bug:            不能处理过大的数
**************************************************************************/
#include "cPredictFilter.h"


cPredictFilter::cPredictFilter()
{
	Tc = 0.0005;
	g = 9.802;
	Kd = 0.1480;
	Km = 0.008;

	aa = 1 / 5.0;
	et = 0.93;
	q = MatrixXf::Identity(6, 6);

	P = MatrixXf::Identity(6, 6);
	R = MatrixXf::Identity(6, 6);

	A = MatrixXf(6, 6);
	A1 = MatrixXf(6, 6);
	B = VectorXf(6);


	isInited = false;

}


cPredictFilter::~cPredictFilter()
{
}


/*************************************************
Function:				cPredictFilter::predict_OneStep
Description:			预测一步（包含反弹）
Calls:                  
Table Accessed:
Table Updated:
Input:                  S_in  预测的初始状态
Output:					S_out, A(若有反弹，会有改变)
Return:					预测的球的新状态
Others:					不导出
*************************************************/
VectorXf cPredictFilter::predict_OneStep(VectorXf S_in)
{
	VectorXf S_out(6);
	V << S_in(3), S_in(4), S_in(5); //取得速度	
	W << 0, 0, 0;
	double P1;
	P1 = Kd*(V.norm())*Tc;

	accelerMatrix <<	1 - P1,        -Km*W(2)*Tc,       Km*W(1)*Tc, 
						Km*W(2)*Tc,     1 - P1,          -Km*W(0)*Tc, 
						- Km*W(1)*Tc,  Km*W(0)*Tc,         1 - P1;


	A << MatrixXf::Identity(3, 3), Tc*MatrixXf::Identity(3, 3),
		 MatrixXf::Zero(3, 3),     accelerMatrix;
	B << 0, 0, 0, 0, 0, -g*Tc;
	
	S_out = A*S_in + B;

	if (S_out(3) > 0)
	{
		return S_out;
	}
	else  //反弹
	{
		A1 << MatrixXf::Identity(3, 3),   Tc*MatrixXf::Identity(3, 3), 
			  MatrixXf::Zero(3, 3),       (Matrix3f() << 1 - aa, 0, 0, 0, 1 - aa, 0, 0, 0, -et);
		S_out = A1*S_out;
		A = A1*A;
	}
	return S_out;
}

/*************************************************
Function:				cPredictFilter::predict_DT
Description:			预测一段时间后球的状态
Calls:                  cPredictFilter::predict_OneStep
Table Accessed:
Table Updated:
Input:					S_in	初始状态， 
						dT		间隔时间, 
Output:					pA_DT 递推累积增益 , pQ 递推累积误差协方差
Return:					预测的球的新状态
Others:					不导出
*************************************************/
VectorXf cPredictFilter::predict_DT(VectorXf S_in, double dT, MatrixXf* pA_DT, MatrixXf* pQ)
{
	VectorXf S_temp = S_in;
	MatrixXf A_DT = MatrixXf::Identity(6, 6);
	MatrixXf Q    = MatrixXf::Identity(6, 6);
	double dt = 0;
	while (dt < dT)
	{
		S_temp = predict_OneStep(S_temp);
		dt = dt + Tc;
		A_DT = A*A_DT;

		//Q = A_DT*Q*A_DT.transpose() + q;  
		Q = Q + q;//这里的处理是为了简化计算，节省时间
	}
	*pA_DT = A_DT;
	*pQ = Q;
	return S_temp;
}


/*************************************************
Function:				cPredictFilter::predict_hit
Description:			预测一段时间后球的状态
Calls:                  cPredictFilter::predict_OneStep
Table Accessed:
Table Updated:
Input:					S_in	初始状态，
						x_hit	击打平面x轴坐标,
Output:					S_out   击打时刻球的状态
						t_out	击打时刻的全局时间
Return:					预测的球的新状态
Others:					不导出
*************************************************/
VectorXf cPredictFilter::predict_hit(VectorXf S_in,double x_hit)
{
	VectorXf S_temp = S_in;
	double dt = 0;
	while (S_in(3) < x_hit && dt < 2)
	{
		S_temp = predict_OneStep(S_temp);
		dt = dt + Tc;
	}
	return S_temp;
}

/*************************************************
Function:				cPredictFilter::filter
Description:			滤波
Calls:                  cPredictFilter::predict_DT
Table Accessed:
Table Updated:
Input:					
Output:					
Return:					预测的球的新状态
Others:					不导出
*************************************************/
bool cPredictFilter::filterA(VectorXf S_in, double t_in)
{
	if (!isInited)
	{
		S = S_in;
		t_last = t_in;
		isInited = true;
	}
	else
	{
		double dT = t_in - t_last;
		//cout << "dT= "<<dT << endl;
		if (dT > 0.1)
		{
			S = S_in;
			t_last = t_in;
			isInited = true;
			return true;
		}
		MatrixXf A_DT(6, 6);
		MatrixXf Q_DT(6, 6);
		//这里H为单位阵，所以省略掉了
		//~~~~~~~~~~~~~~~~~~~~~~~
		// Predict
		//   Predicted state estimate
		VectorXf S_pre = predict_DT(S, dT, &A_DT, &Q_DT); //
		//   Predicted covariance estimate
		MatrixXf P_pre = A_DT*P*A_DT.transpose() + Q_DT;

		//~~~~~~~~~~~~~~~~~~~~~~~

		// Update
		//Innovation or measurement residual
		MatrixXf y = S_in - S_pre;
		//Innovation(or residual) covariance
		MatrixXf Sc = P + R;
		//Near - optimal Kalman gain
		MatrixXf K = P * Sc.inverse();
		//Updated covariance estimate
		P = P_pre - K * P_pre;
		//Updated state estimate
		S = S_pre + K * y;

		t_last = t_in;

	}

	return true;
	
}