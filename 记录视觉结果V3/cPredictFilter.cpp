/**************************************************************************
Copyright:		JR.Liu
Author:			����Ƚ
Date:			2015-9-9
Description:	���󻯵���켣Ԥ��
platform:       VS2013
Bug:            ���ܴ���������
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
Description:			Ԥ��һ��������������
Calls:                  
Table Accessed:
Table Updated:
Input:                  S_in  Ԥ��ĳ�ʼ״̬
Output:					S_out, A(���з��������иı�)
Return:					Ԥ��������״̬
Others:					������
*************************************************/
VectorXf cPredictFilter::predict_OneStep(VectorXf S_in)
{
	VectorXf S_out(6);
	V << S_in(3), S_in(4), S_in(5); //ȡ���ٶ�	
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
	else  //����
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
Description:			Ԥ��һ��ʱ������״̬
Calls:                  cPredictFilter::predict_OneStep
Table Accessed:
Table Updated:
Input:					S_in	��ʼ״̬�� 
						dT		���ʱ��, 
Output:					pA_DT �����ۻ����� , pQ �����ۻ����Э����
Return:					Ԥ��������״̬
Others:					������
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
		Q = Q + q;//����Ĵ�����Ϊ�˼򻯼��㣬��ʡʱ��
	}
	*pA_DT = A_DT;
	*pQ = Q;
	return S_temp;
}


/*************************************************
Function:				cPredictFilter::predict_hit
Description:			Ԥ��һ��ʱ������״̬
Calls:                  cPredictFilter::predict_OneStep
Table Accessed:
Table Updated:
Input:					S_in	��ʼ״̬��
						x_hit	����ƽ��x������,
Output:					S_out   ����ʱ�����״̬
						t_out	����ʱ�̵�ȫ��ʱ��
Return:					Ԥ��������״̬
Others:					������
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
Description:			�˲�
Calls:                  cPredictFilter::predict_DT
Table Accessed:
Table Updated:
Input:					
Output:					
Return:					Ԥ��������״̬
Others:					������
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
		//����HΪ��λ������ʡ�Ե���
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