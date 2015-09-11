
/**************************************************************************
Copyright:		JR.Liu
Author:			����Ƚ
Date:			2015-9-08
Description:	kalman�˲���C++ʵ��
Bug:            ���ܴ���������
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
Description:			���ϵͳ�ڸ�����ά���Ƿ���ȷ
Calls:                  cSystemInform::InitSystem,
Table Accessed:
Table Updated:
Input:
Output:
Return:					�Ƿ��ʼ���ɹ�
Others:					������
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
	Description:			��ʼ��ϵͳ��Ϣ
	Calls:
	Table Accessed:
	Table Updated:
	Input:					������������Ϣ: a ת�ƾ��� b ƫ�ƾ��� �� h �۲���� �� q ģ�ͷ�� p �ۻ������� r �۲�����
	Output:
	Return:					�Ƿ��ʼ���ɹ�
	Others:					������
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
	Description:			�����µĹ۲����ݸ���ϵͳ״̬��kalman�˲��ĺ��Ĺ�ʽ
	Calls:
	Table Accessed:
	Table Updated:
	Input:					z_new; �µĹ۲�ֵ
	Output:					isSucces (bool)
	Return:					�Ƿ��ʼ���ɹ�
	Others:					������
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