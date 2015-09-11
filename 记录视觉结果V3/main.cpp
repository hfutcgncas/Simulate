#include "stdio.h"
#include <iostream>
#include "camer.h"
#include "Cam3D.h"

#include <io.h> //_access
#include"string.h"
#include "direct.h" // mkdir
#include "time.h" // time

#include "conio.h"

#include <vector>
#include "cRobotArm.h"
#include "GlobleSrouce.h"

#include "time.h" // time
#include <Winsock2.h>

#include "cPredictFilter.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;


//#define		CamA_IP		"192.168.64.88"
//#define		CamA_PORT		2002
//#define		CamB_IP		"192.168.64.89"
//#define		CamB_PORT		2002
//#define		CamC_IP		"192.168.64.98"
//#define		CamC_PORT		2002
//#define		CamD_IP		"192.168.64.99"
//#define		CamD_PORT		2002

#define		CamA_IP		"127.0.0.1"
#define		CamA_PORT		2002
#define		CamB_IP		"127.0.0.1"
#define		CamB_PORT		2003
#define		CamC_IP		"127.0.0.1"
#define		CamC_PORT		2004
#define		CamD_IP		"127.0.0.1"
#define		CamD_PORT		2005

//#define		UseFilter

//globle ver--------------------
extern cClock SystemClock;
extern cFile  dataFile;
//------------------------------

bool CheckBallStatueChange(sBallStatue BallStatue,sBallStatue BallStatue_last);
bool CheckTraceChange(double x_out,double y_out,double z_out,double vx_out,double vy_out,double vz_out,double t_out);



//-----------------------------------------
static cHis3DTemp ballPosHis,ballDoubtPosHis,ballPredictHis;
static sBallStatue BallStatue,BallStatue_Last,BallStatue_doubt_Last;
static int DoultCount;

static int PreCount;

PrediectGroup CurrentPrediect;
PrediectGroup HisPrediect;

//==========================
//==========================
void main()
{
	double t_0,t_c;
	cRobotArm	robotArm;
	cCam3D      robotVision;
	cShell      shell;

	cBallModel  ballModel;
	cPrediectOutput PrediectorFil;

	BallPoint startPoint;
	BallPoint hitPoint;


	cPredictFilter PredictFilter;


	double ballHit[7];

	int Predicted = 0;

	double t_1, t_2;
	
	ballPosHis.clear();
//��ʼ�����ʱ----------------------
//--------------------------------------------
	dataFile.FileInit();									//���ݴ洢�ĳ�ʼ��
	robotVision.Init(CamA_IP, CamA_PORT, CamB_IP, CamB_PORT, CamC_IP, CamC_PORT, CamD_IP, CamD_PORT);	//���������ĳ�ʼ��
	robotArm.ComInit();										//���˶�������ͨ�ŵĳ�ʼ��
	InitMatrax();//////////

	printline(30);
    //-----------------------------------
	t_0 = SystemClock.now();
	while(shell.getCommand()!='q') //��ѭ��
	{
		while (SystemClock.now() - t_0 < 5)  //������ʱ���ȴ�ϵͳ��ʼ��
		{
		}
		//t_o = SystemClock.now();  //ʱ�Ӷ��ڸ�λ

		double t_out,x_out,y_out,z_out,vx_out,vy_out,vz_out;
		t_out = SystemClock.now()/1000.0;
		//if (t_out > 1000)
		//{
		//	robotVision.Sync();
		//	continue;
		//}	
		char rt = robotVision.RefreshSrcData(); //�����µ�����ͷ����
		if( rt!= 0  && SystemClock.now() - t_0 > 5 ) //
		{
			t_0 = SystemClock.now();
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #pragma region Reconstruct
			robotVision.ReConstruction(rt,&t_out,&x_out,&y_out,&z_out,&vx_out,&vy_out,&vz_out);//��ά�ؽ�

			//��������ϵԭ��,����궨ԭ�������������ģ�ת�����ϳ����ԭ��
			x_out = x_out + 1100;
			y_out = y_out + 300;

			if ( dataFile.m_bStartSave )   //�洢ƹ����λ��
			{
				dataFile.SaveTraceData(x_out, y_out, z_out, t_out, vx_out, vy_out, vz_out, 88);
			} 
			//��λת�� תΪ m��m/s��s
			startPoint.x = x_out / 1000; startPoint.y = y_out / 1000; startPoint.z = z_out / 1000;
			startPoint.Vx = vx_out / 1000; startPoint.Vy = vy_out / 1000; startPoint.Vz = vz_out / 1000;
			startPoint.Wx = 0; startPoint.Wy =0; startPoint.Wz = 0;
			startPoint.t = t_out;
			//��֤���״̬�Ƿ���ҪԤ��,���������Բ��ԣ���������
			if (startPoint.x < 0 || startPoint.x > 2
				|| startPoint.y <-0.75 || startPoint.y > 0.75
				|| startPoint.z <-0.05 || startPoint.z > 2.0
				|| startPoint.Vx < 0)
			{
				continue;
			}
            #pragma endregion
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #pragma region Predict & Filter
			/* //�ϰ��Ԥ�����
			double confidence;
			int rt2 = ballModel.predict(startPoint, &hitPoint, &confidence,2);
			if (rt2 == PREDICT_NORMAL || rt2 == PREDICT_NOREBOND)
			{
				CurrentPrediect.Ball_Start.Copy(startPoint);
				CurrentPrediect.Ball_Stop.Copy(hitPoint);
				CurrentPrediect.Confidence = confidence;
			}*/

			VectorXf S_in(6);
			S_in << startPoint.x, startPoint.y, startPoint.z, startPoint.Vx, startPoint.Vy, startPoint.Vz;
			
#ifdef UseFilter
			PredictFilter.filterA(S_in, startPoint.t);
			VectorXf S_out = PredictFilter.predict_hit(PredictFilter.S, 2); //ʵ��������³����Ԥ���׼ȷ
#else
			VectorXf S_out = PredictFilter.predict_hit(S_in, 2); //ʵ��������³����Ԥ���׼ȷ
#endif
			t_2  = SystemClock.now();
//			hitPoint.t -= (t_2 - t_0)/1000; //��ȥ��������ʱ�䡣  Ϊ0.1ms��


			cout << (t_2 - t_0) / 1000 << endl;

            #pragma endregion
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			#pragma region SendCmd
			//switch (rt2)
			//{
			//case PREDICT_NORMAL:
			//	if (startPoint.x < 1.8)
			//	{
			//		cout << "send1: " << hitPoint.t << endl
			//			<<hitPoint.y<<endl;
			//	//	robotArm.HitBall(hitPoint.x, hitPoint.y, hitPoint.z, hitPoint.Vx, hitPoint.Vy, hitPoint.Vz, hitPoint.t);
			//	}
			//	break;
			//case PREDICT_NOREBOND:
			//	if (startPoint.x > 1.1)
			//	{
			//		cout << "send2: " << hitPoint.t << endl 
			//			<< hitPoint.y << endl;
			//	//	robotArm.HitBall(hitPoint.x, hitPoint.y, hitPoint.z, hitPoint.Vx, hitPoint.Vy, hitPoint.Vz, hitPoint.t);
			//	}
			//	break;
			//case PREDICT_TIMEOUT:
			//	cout << "send3: Ԥ��ʧ��,��ʱ"  << endl;
			//	break;
			//case PREDICT_TOOSHORT:
			//	cout << "send4: ������" << endl;
			//	break;
			//default:
			//	break;
			//}
			//cout << "----";
			
		//	if (rt2 == 1 || rt2 == 2)
		//	{
		//		cout << "time:"<<hitPoint.y << endl
		//			<< rt2 << endl;
		//	}
		//	else
		//	{
		//		cout << "fail" <<rt2<< endl;
		//	}
		//	t_2 = SystemClock.now() / 1000.0;
		////	cout << "���"<<t_2 - t_1 << endl;
		//	cout <<"============================"<< endl;


			//if (CheckTraceChange(x_out,y_out,z_out,vx_out,vy_out,vz_out,t_out))//�������¹켣
			//{
			//	cout<<"Prepare to Predict"<<endl;
			//	Predicted = 0;	
			//	ballPredictHis.clear();
			//}
			//if ( Predicted == 0 )
			//{
			//	ballPredictHis.push_back(x_out,y_out,z_out,vx_out,vy_out,vz_out,t_out);
			//	if ( ballPredictHis.size() >= 5 )
			//	{
			//		
			//		//Predict_traceV2(ballPredictHis,ballHit);
			//		Predicted = 1;

			//		//robotArm.HitBall(ballHit[0],ballHit[1],ballHit[2],ballHit[3],ballHit[4],ballHit[5],ballHit[6]*1000);

			//		for (int i = 0;i<7;i++)
			//		{
			//			cout<<ballHit[i]<<" ";
			//		}
			//		cout<<endl;
			//		printline(30);
			//		ballPredictHis.clear();
			//	}	
			//}
			#pragma endregion
	    }
	}

	return;

}
//--------------------------------------------------------------------
//
//
//
//
//
//
//-------------------------------------------------------------------
//==================================
bool CheckBallStatueChange(sBallStatue BallStatue,sBallStatue BallStatue_last)
{
	if(BallStatue.direction != BallStatue_last.direction)return 1;//����ı�}
	if(BallStatue.direction == 1 && (BallStatue.x_last < BallStatue_last.x_last))return 1;//������켣λ�ò���
	if(BallStatue.direction == 0 && (BallStatue.x_last > BallStatue_last.x_last))return 1;//������켣λ�ò���
	if(   BallStatue.t_last - BallStatue_last.t_last > 1 
		||BallStatue.t_last - BallStatue_last.t_last < 0 )return 1;//ʱ��������1s
	return 0;
}
//==================================
bool CheckTraceChange(double x_out,double y_out,double z_out,double vx_out,double vy_out,double vz_out,double t_out)
{
	BallStatue.x_last = x_out;  //BallStatue��ʾ��ǰ״̬
	BallStatue.t_last = t_out;
	BallStatue.direction = (vx_out>0);

	if(!CheckBallStatueChange(BallStatue,BallStatue_Last)) //��ƹ����켣δ�䣬����BallStatue_Last
	{
		ballDoubtPosHis.clear();

		BallStatue_Last.x_last = x_out;
		BallStatue_Last.t_last = t_out;
		BallStatue_Last.direction = (vx_out>0);


		if (dataFile.m_bStartSave && vx_out > 0 )   //�洢ƹ����λ��
		{
			dataFile.SaveTraceData( x_out  , y_out , z_out  , t_out ,vx_out,vy_out,vz_out,88);
		}  

		if( ballPosHis.size()< 5  )  //����ʷ��¼����С��5 ��׷����ʷ����
		{
			ballPosHis.push_back(x_out,y_out,z_out,vx_out,vy_out,vz_out,t_out);
		}
		else if(  BallStatue_Last.predicted == 0) //�������㹻�����л���׼��Ԥ���״̬
		{
			BallStatue_Last.predicted = 1;
			cout<<"New trace "<<PreCount++<<endl;

			if( BallStatue_Last.direction == 1 )
			{
				return true;
			}
		}
	}
	else //���б仯�����¼�����仯�������������仯��4��ʱ������ballPosHis�����ballDoubtPosHis��������BallStatue_Last
	{
		if( ballDoubtPosHis.size() <= 4 )
		{
			if( !CheckBallStatueChange(BallStatue,BallStatue_doubt_Last) ) //����ǰ�켣����ɹ켣״̬һ�£���׷�ӿ��ɹ켣����
			{
				ballDoubtPosHis.push_back(x_out,y_out,z_out,vx_out,vy_out,vz_out,t_out);
				BallStatue_doubt_Last.x_last = x_out;
				BallStatue_doubt_Last.t_last = t_out;
				BallStatue_doubt_Last.direction = (vx_out>0);
			}
			else //����ǰ�켣����ɹ켣״̬��һ�£������֮ǰ�Ŀ��ɹ켣�洢��
			{
				ballDoubtPosHis.clear();
				ballDoubtPosHis.push_back(x_out,y_out,z_out,vx_out,vy_out,vz_out,t_out);
				BallStatue_doubt_Last.x_last = x_out;
				BallStatue_doubt_Last.t_last = t_out;
				BallStatue_doubt_Last.direction = (vx_out>0);
			}
		}
		else//������5�����쳣ʱ����Ϊ�켣�ı�
		{
			ballPosHis.copy(ballDoubtPosHis);
			BallStatue_Last.predicted = 0;
			ballDoubtPosHis.clear();

			BallStatue_Last.x_last = x_out;
			BallStatue_Last.t_last = t_out;
			BallStatue_Last.direction = (vx_out>0);

			if (dataFile.m_bStartSave)
			{
				dataFile.SaveTraceData(0,0,0,0,0,0,0,1);//�رյ�ǰ�ļ����Դ����µĹ켣�ļ�
				cout<<"NewFile"<<endl;

			//	dataFile.SaveTraceData(1,2,3,4,5,6,7,88);
			/*	for (int M =0;M<ballPosHis.size();M++) //�����ļ��м�¼�ı�ǰ���ɹ켣�ڵļ�¼
				{
					dataFile.SaveTraceData(ballPosHis.tempX[M],ballPosHis.tempY[M],ballPosHis.tempZ[M],ballPosHis.tempTime[M],ballPosHis.tempVx[M],ballPosHis.tempVy[M],ballPosHis.tempVz[M],88);
				}*/
			}
		}
	}
	return 0;
}



