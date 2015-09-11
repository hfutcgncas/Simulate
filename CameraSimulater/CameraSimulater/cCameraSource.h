#pragma once
#include "InitSock.h"
#include "TCPListener.h"
#include"cImgCoordinateStruct.h"

DWORD WINAPI ThreadProc(LPVOID lpParam);
class cCameraSource
{
public:

	
	cCameraSource(char* ip, u_short port, cImgCoordinateStruct camdata);
	~cCameraSource();

	cTCPListener camListener;
	cImgCoordinateStruct camData;

	HANDLE hThread1, hThread2;
	DWORD ThreadID;
	char state;

	void CamOn(void);
	
	void CameraThread();
};

