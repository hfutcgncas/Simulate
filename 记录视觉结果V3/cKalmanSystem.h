/**************************************************************************
Copyright:		JR.Liu
Author:			����Ƚ
Date:			2015-9-08
Description:	kalman�˲���C++ʵ�� 
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


class cKalmanSystem
{
public:
	cKalmanSystem();
	~cKalmanSystem();

private:
	int x_Dim;   //״̬����ά��
	int z_Dim;	 //�۲�����ά��
	int u_Dim;
public:
	MatrixXd A;  // transition matrix ת�ƾ���
	MatrixXd B;  // x     =  A x  +  B  u
	VectorXd x;  //  i+1        i        i+1	
	MatrixXd P;  //ϵͳ���

	MatrixXd H;  // �۲����
	MatrixXd Q;  // z     =  H x  
	VectorXd z;  //  i          i
	MatrixXd R;  //�۲����

	VectorXd u;  //ϵͳ�����ƫ��
	MatrixXd K;  //����

#pragma region functionDef  
private:
	/*************************************************
	Function:				cSystemInform::checkSystem
	Description:			���ϵͳ�ڸ�����ά���Ƿ���ȷ
	*************************************************/
	bool checkSystem();
public:
	/*************************************************
	Function:				cSystemInform::InitSystem
	Description:			��ʼ��ϵͳ��Ϣ
	*************************************************/
	bool InitSystem(MatrixXd a, MatrixXd b, MatrixXd h, MatrixXd q, MatrixXd p, MatrixXd r, VectorXd x_init, VectorXd u_init);
	/*************************************************
	Function:				cSystemInform::upDateState
	Description:			�����µĹ۲����ݸ���ϵͳ״̬��kalman�˲��ĺ��Ĺ�ʽ
	*************************************************/
	bool upDateState(VectorXd z_new);
#pragma endregion 


};
