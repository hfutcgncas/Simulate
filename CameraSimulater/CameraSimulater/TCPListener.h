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

	char recvBuf[20];			// ����յ�����Ϣ
	char sendBuf[20];			// ��ŷ��͵���Ϣ


	cTCPListener(char* ip, u_short port); //���캯��	
	cTCPListener(); //���캯��	

	char SocketInit(void);//��ʼ������
	bool acceptLink(void);//�ȴ�client����
	void closeLink(void);//�ر�����

	void RecvData();
	void SendData();

};