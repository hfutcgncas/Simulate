#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "InitSock.h"
#include <stdio.h>




class cTCPListener
{
public:
	char* IP;
	u_short PORT;

	SOCKET sListen;
	sockaddr_in sin;


	SOCKET sClient;

	char recvBuf[20];			// 存放收到的信息
	char sendBuf[20];			// 存放发送的信息


	cTCPListener(char* ip, u_short port); //构造函数	
	cTCPListener(); //构造函数	

	char SocketInit(void);//初始化网络
	bool acceptLink(void);//等待client链接
	void closeLink(void);//关闭链接

	void RecvData();
	void SendData();

};