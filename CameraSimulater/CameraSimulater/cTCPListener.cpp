#pragma once

#include "TCPListener.h"

cTCPListener::cTCPListener()
{

}

cTCPListener::cTCPListener(char* ip, u_short port)
{
	PORT = port;
	IP = ip;
}
char cTCPListener::SocketInit()
{
	//�����׽���
	sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		printf("Failed socket()\n");
		return 0;
	}
	//���sockaddr_in�ṹ
	//sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//������׽��ֵ�һ�����ص�ַ
	if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind()\n");
		return 0;
	}
	//�������ģʽ
	if (::listen(sListen, 2) == SOCKET_ERROR)
	{
		printf("Failed listen()\n");
		return 0;
	}
	//ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	printf("Length of sockaddr_in is: %d\n", nAddrLen);
	
}

bool cTCPListener::acceptLink(void)
{
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);

	sClient = ::accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrLen);
	if (sClient == INVALID_SOCKET)
	{
		printf("Failed accapt()");
		return false;
	}
	printf("���ܵ�һ������:%s\r\n", inet_ntoa(remoteAddr.sin_addr));
	return true;
}

void cTCPListener::closeLink(void)
{
	::closesocket(sListen);
}

void cTCPListener::RecvData()
{
	int nRev = ::recv(sClient, recvBuf, 3, 0);
}
void cTCPListener::SendData()
{
	::send(sClient, sendBuf, 20, 0);
}