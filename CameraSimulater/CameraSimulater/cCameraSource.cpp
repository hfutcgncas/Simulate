#include "cCameraSource.h"
#include "cClock.h"
#include "iostream"

using namespace std;

extern cClock sysClock;
extern char cmd;
extern double t_start;

cCameraSource::cCameraSource(char* ip, u_short port, cImgCoordinateStruct camdata)
{
	camListener = cTCPListener(ip, port);
	camListener.SocketInit();
	camData.copy(camdata);
}


cCameraSource::~cCameraSource()
{
}

void cCameraSource::CamOn(void)
{
	//-----------------------------------------------
	printf("camOn\n");
	hThread1 = CreateThread(NULL,
		0,
		ThreadProc,
		(void *)this,
		0,
		NULL);
	return;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	cCameraSource *pDlg = (cCameraSource*)lpParam;
	pDlg->CameraThread();
	return 1;
}


void cCameraSource::CameraThread()
{
	int* p;
	while (TRUE)
	{
		camListener.acceptLink();
		
		//以对时为开始信号
		while (true)
		{
			camListener.RecvData();
			if (camListener.recvBuf[0] == 'T' && camListener.recvBuf[1] == 'C')
			{
				p = (int*)(&camListener.sendBuf[0]);
				*p = 2;
				sysClock.reset();
				camListener.SendData();
				break;
			}

		}
		
		while (TRUE)
		{
			if (state != 's')
			{
				camListener.RecvData();
				p = (int*)(&camListener.sendBuf[0]);
				*p = 0;
				camListener.SendData();
				//cout << "state0" << endl;
			}
			else
			{
				int i = 0;
				while (i < camData.size())
				{
					camListener.RecvData();
					if (sysClock.now() < camData.t[i] + t_start)
					{
						p = (int*)(&camListener.sendBuf[0]);
						*p = 0;
						camListener.SendData();
						//cout << "state1" << endl;
					}
					else
					{
						p = (int*)(&camListener.sendBuf[0]);
						*p = 1;
						p = (int*)(&camListener.sendBuf[4]);
						*p = camData.u[i];
						p = (int*)(&camListener.sendBuf[8]);
						*p = camData.v[i];
						p = (int*)(&camListener.sendBuf[12]);
						*p = ((int)(camData.t[i] + t_start)) / 1000;
						//*p = ((int)sysClock.now()*1000) / 1000;
						p = (int*)(&camListener.sendBuf[16]);
						*p = ((int)(camData.t[i] + t_start)) % 1000;
						//*p = ((int)sysClock.now() * 1000) %1000;

						camListener.SendData();
						//cout << "state2" << endl;
						//Sleep(3);
						i++;
					}
				}
				state = '\0';				
			}
		}


		//while ( state != 's')
		//{
		//	camListener.RecvData();
		//	p = (int*)(&camListener.sendBuf[0]);
		//	*p = 0;
		//	camListener.SendData();
		//}
		//
		//
		//while (i<camData.size())
		//{
		//	camListener.RecvData();
		//	//if (camListener.recvBuf[0] == 'T' && camListener.recvBuf[1] == 'C')
		//	//{
		//	//	p = (int*)(&camListener.sendBuf[0]);
		//	//	*p = 2;
		//	//	sysClock.reset();
		//	//	camListener.SendData();
		//	//}
		//	//else
		//	//{
		//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//		if (sysClock.now() < camData.t[i])
		//		{
		//			p = (int*)(&camListener.sendBuf[0]);
		//			*p = 0;
		//			camListener.SendData();
		//		}
		//		else
		//		{
		//			p = (int*)(&camListener.sendBuf[0]);
		//			*p = 1;
		//			p = (int*)(&camListener.sendBuf[4]);
		//			*p = camData.u[i];
		//			p = (int*)(&camListener.sendBuf[8]);
		//			*p = camData.v[i];
		//			p = (int*)(&camListener.sendBuf[12]);
		//			*p = ((int)(camData.t[i] + t_start)) / 1000;
		//			//*p = ((int)sysClock.now()*1000) / 1000;
		//			p = (int*)(&camListener.sendBuf[16]);
		//			*p = ((int)(camData.t[i] + t_start)) % 1000;
		//			//*p = ((int)sysClock.now() * 1000) %1000;

		//			camListener.SendData();
		//			Sleep(3);
		//			i++;
		//		}
		//	//}
		//}
		printf("waitting for new connect\n");
	}
}