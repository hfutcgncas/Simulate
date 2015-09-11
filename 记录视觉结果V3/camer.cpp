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
void cleanBuf(char *buf , int len)//把存储空间擦除
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
cCamer::cCamer(char* IP,u_short port)//构造函数，用于赋初值
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
//1、建立本PC与摄像头的网络连接
//========================================================
char cCamer::CamNetInit()
{
	WORD wVersionRequested;
    WSADATA wsaData;//WSAata用来存储系统传回的关于WinSocket的资料。
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
	sockClient=socket(AF_INET,SOCK_STREAM,0);// AF_INET ..tcp连接
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP);//传感器IP
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(port);// 设置端口号
	
	err=ioctlsocket(sockClient,FIONBIO,(unsigned long *)&ul);//设置成非阻塞模式。  
	if(err==SOCKET_ERROR)//设置失败。  
	{  
		cout<<"net_init fail"<<endl;
	} 


	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//连接服务器
	
	return 1;
}

//========================================================
//2、独立线程的方式（当前未使用）
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
//3、与摄像头通过TCP/IP进行通讯
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
		else if(err==WSAETIMEDOUT)//超时。  
		{
			return 0;
		}  
		else if(err==WSAENETDOWN)//连接断开。  
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
      else if(err==WSAETIMEDOUT)//超时。  
	  {
			dataReadFlag = 0;
			return 0;
	  }  
      else if(err==WSAENETDOWN)//连接断开。  
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
//4、与摄像头对时，使摄像头时间归0 (自定义协议：PC->CAM “TC”，CAM时钟归0，并返回”2，0，0，...“)
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
//5、更新乒乓球图像坐标的循环链表camData，当前未使用
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
//6、乒乓球图像坐标的自身滤波
//========================================================
void cCamer::CamFilter(  )
{
	int index = 0;
	double timeNow = SystemClock.now();
	while(FilterTemp.size() != 0 ) //丢弃存储区中过老的数据
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
	if(dataReadFlag)//若收到新的数据，则更新FilterTemp
	{
		FilterTemp.push_back(x_cur,y_cur,t_cur);
		dataReadFlag = 0;
	}
	if( FilterTemp.size() > 5 )//若存有足够的历史数据，则认为可以继续重建
	{
		FilterState = 1;
	}
	else
		FilterState = 0;
	return;
}

//========================================================
//7、乒乓球图像坐标的队列实现
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

