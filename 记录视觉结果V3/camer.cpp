#include "camer.h"
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "stdio.h"
#include "windows.h"
#include "GlobleSrouce.h"
#include <iostream>

using namespace std;

extern cClock SystemClock;


///////////////////////////////////////////////////////////
void cleanBuf(char *buf , int len)//�Ѵ洢�ռ����
{
	int i;
	for( i= 0;i<len;i++ )
	{
		buf[i] = 0x00;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================
cCamer::cCamer(void)
{
	busyFlag = FALSE;
}
cCamer::cCamer(char* IP,u_short port)//���캯�������ڸ���ֵ
{
	this->IP = IP;
	this->port = port;

	cur_index = 1;
	last_index = 0;
	New_Data_Rcv = 0;
	index = 0;
	FilterState = 0;

	busyFlag = FALSE;

	pz1 = newMatrix(4,1);
	pz2 = newMatrix(4,1);
	pz3 = newMatrix(4,1);
	pz4 = newMatrix(4,1);
	 pH = newMatrix(4,2);
	pgls1 = newMatrix(2,1);
	pgls2 = newMatrix(2,1);

}
cCamer::~cCamer()
{
	//freeMatrix(pz1);
	//freeMatrix(pz2);
	//freeMatrix(pz3);
	//freeMatrix(pz4);
	//freeMatrix(pH);
	//freeMatrix(pgls1);
	//freeMatrix(pgls2);
}

//========================================================
//1��������PC������ͷ����������
//========================================================
char cCamer::CamNetInit()
{
	WORD wVersionRequested;
    WSADATA wsaData;//WSAata�����洢ϵͳ���صĹ���WinSocket�����ϡ�
    int err;    
	printf("camInit\n");
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return	0;
    }
    
    if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) 
    {
        WSACleanup( );
        return	0;
    }
	
	unsigned long ul=1; 


	cleanBuf(recvBuf,LENTH_OF_RECBUF);		
	sockClient=socket(AF_INET,SOCK_STREAM,0);// AF_INET ..tcp����
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP);//������IP
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(port);// ���ö˿ں�
	
	err=ioctlsocket(sockClient,FIONBIO,(unsigned long *)&ul);//���óɷ�����ģʽ��  
	if(err==SOCKET_ERROR)//����ʧ�ܡ�  
	{  
		cout<<"net_init fail"<<endl;
	} 


	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//���ӷ�����
	
	return 1;
}

//========================================================
//2�������̵߳ķ�ʽ����ǰδʹ�ã�
//========================================================
void cCamer::CamOn(void)
{
	//-----------------------------------------------
	printf("camOn\n");
	hThread1=CreateThread(NULL,
							0,
							ThreadProc,
							(void *)this,
							0,
							NULL);
	return;
}

DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{
	cCamer *pDlg = (cCamer*)lpParam;
	pDlg->Camthread();
	return 1;
}

char cCamer::Camthread()
{
	int vail=0;
	while(1)
	{
		Communicate_2_Cam("ok",recvBuf);
		vail = *(int*)recvBuf;
		if (vail == 1)
		{
			Refreash_Data();			
			New_Data_Rcv = 1;
		}		
	}
	return 1;
}

//========================================================
//3��������ͷͨ��TCP/IP����ͨѶ
//========================================================
bool cCamer::Communicate_2_Cam( char * sendBuf , char * recvBuf )
{
	cleanBuf(recvBuf,LENTH_OF_RECBUF);
	//printf("send:  %s\n",sendBuf);
	send(sockClient,sendBuf,(int)strlen(sendBuf)+1,0);
	int ret ;
	long err;
	ret = recv(sockClient,recvBuf,LENTH_OF_RECBUF,0);
	//printf("return:  %s\n\n\n",recvBuf1);	
	while(ret==SOCKET_ERROR)  
	{  
		err=WSAGetLastError();  
		if(err==WSAEWOULDBLOCK)  
		{
		}  
		else if(err==WSAETIMEDOUT)//��ʱ��  
		{
			return 0;
		}  
		else if(err==WSAENETDOWN)//���ӶϿ���  
		{
			return 0;
		} 
		ret = recv(sockClient,recvBuf,LENTH_OF_RECBUF,0);
	} 
	return 1;
}

bool cCamer::AskForData()
{

	double t = SystemClock.now();
	char sendtime[3];
	
		sendtime[0] = (char)((int)t/1000);
		sendtime[1] = (char)((int)t%1000);
		sendtime[2] = 0;
		send(sockClient,sendtime,3,0);
		

	return 1;
}
bool cCamer::GetForData(double SysTime)
{
	char recvbuf[LENTH_OF_RECBUF];
	recvbuf[0] = 0;
	recvbuf[1] = 0;
	recvbuf[2] = 0;
	recvbuf[3] = 0;
	int ret;
	long err;
	ret = recv(sockClient,recvbuf,LENTH_OF_RECBUF,0);
	
	if(ret==SOCKET_ERROR)  
   {  
  
      err=WSAGetLastError();  
  
      if(err==WSAEWOULDBLOCK)  
	  {
			dataReadFlag = 0;
			return 0;
	  }  
      else if(err==WSAETIMEDOUT)//��ʱ��  
	  {
			dataReadFlag = 0;
			return 0;
	  }  
      else if(err==WSAENETDOWN)//���ӶϿ���  
	  {
			dataReadFlag = 0;
			return 0;
	  } 
   } 
   else
   {
	   	dataReadFlag = (*(int*)(recvbuf));
		x_cur =  (double)(*(int*)(recvbuf+4));
		x_cur /= 10;
		y_cur =  (double)(*(int*)(recvbuf+8));
		y_cur /= 10;
		t_cur =  (double)((*(int*)(recvbuf+16))/1000.0+(*(int*)(recvbuf+12)));
		//t_cur =  SysTime - (double)(*(int*)(recvbuf+12)) ;
		//t_cur /= 1000.0;

		if (x_cur > 640 || x_cur < 0
			|| y_cur > 480 || y_cur < 0)
		{
			dataReadFlag = 0;
			return 0;
		}
		return dataReadFlag;
	}

}
//========================================================
//4��������ͷ��ʱ��ʹ����ͷʱ���0 (�Զ���Э�飺PC->CAM ��TC����CAMʱ�ӹ�0�������ء�2��0��0��...��)
//========================================================
bool cCamer::CamSync()
{
	int rint;
	Communicate_2_Cam("TC",recvBuf);
	{
		rint = *(int*)(recvBuf);
		if (rint == 2)
		{
			printf(IP);
			printf(":TimeSync OK\n");
			return 1;
		} 
		else
		{
			printf(IP);
			printf(":TimeSync Fail\n");
			printf(":%d\n",rint);
			return 0;
		}
	}
}
//========================================================
//5������ƹ����ͼ�������ѭ������camData����ǰδʹ��
//========================================================
char cCamer::Refreash_Data()
{
	cur_index++;
	last_index++;
	if (cur_index == 10)cur_index=0;
	if (last_index == 10)last_index=0;
	camData[cur_index].x =    *(int*)(recvBuf+4);
	camData[cur_index].y =    *(int*)(recvBuf+8);
	camData[cur_index].time = (*(int*)(recvBuf+16))/1000.0+(*(int*)(recvBuf+12));
	index++;
	return 1;
}

//========================================================
//6��ƹ����ͼ������������˲�
//========================================================
void cCamer::CamFilter(  )
{
	int index = 0;
	double timeNow = SystemClock.now();
	while(FilterTemp.size() != 0 ) //�����洢���й��ϵ�����
	{
		if( timeNow/1000 - FilterTemp.tempTime[0] > 0.05
			||  FilterTemp.size() > 8 )
		{
			FilterTemp.del();
		}
		else
		{
		//	cout<<"size = "<<FilterTemp.size()<<endl;
			break;
		}
	}
	if(dataReadFlag)//���յ��µ����ݣ������FilterTemp
	{
		FilterTemp.push_back(x_cur,y_cur,t_cur);
		dataReadFlag = 0;
	}
	if( FilterTemp.size() > 5 )//�������㹻����ʷ���ݣ�����Ϊ���Լ����ؽ�
	{
		FilterState = 1;
	}
	else
		FilterState = 0;
	return;
}

//========================================================
//7��ƹ����ͼ������Ķ���ʵ��
//========================================================
void cTempData::push_back(double x,double y,double time)
{
	tempX.push_back(x);
	tempY.push_back(y);
	tempTime.push_back(time);
}
void cTempData::clear()
{
	tempX.clear();
	tempY.clear();
	tempTime.clear();
}
int cTempData::size()
{
	return (int)(tempX.size());
}
void cTempData::del()
{
	tempX.erase(tempX.begin());
	tempY.erase(tempY.begin());
	tempTime.erase(tempTime.begin());
}
void cTempData::copy(cTempData src)
{
	tempX = src.tempX;
	tempY = src.tempY;
	tempTime = src.tempTime;
}

