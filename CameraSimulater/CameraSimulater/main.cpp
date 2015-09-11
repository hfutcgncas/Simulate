//TCPServer.cpp
#include "InitSock.h"
#include "TCPListener.h"
#include"cFileOperate.h"
#include"cImgCoordinateStruct.h"
#include <stdio.h>

#include "cClock.h"
#include "cCameraSource.h"
#include "cShell.h"

CInitSock initSock; //初始化Winsock库
cClock sysClock;
char cmd;
double t_start;
cShell shell;

int main()
{
	//读取文件记录
	//======================================================================
	cFileOperate File("data.txt");
	int dataSize = File.LoadFullFile();

	cImgCoordinateStruct camAData;
	cImgCoordinateStruct camBData;
	cImgCoordinateStruct camCData;
	cImgCoordinateStruct camDData;

	for (int i = 0; i < dataSize; i++)
	{
		if (File.data_Read[i][2] == 0)
		{
			camAData.push_back(File.data_Read[i][0], File.data_Read[i][1], (File.data_Read[i][3] - File.data_Read[0][3])*1);
		}
		else if (File.data_Read[i][2] == 1)
		{
			camBData.push_back(File.data_Read[i][0], File.data_Read[i][1], (File.data_Read[i][3] - File.data_Read[0][3]) * 1);
		}
		else if (File.data_Read[i][2] == 3)
		{
			camCData.push_back(File.data_Read[i][0], File.data_Read[i][1], (File.data_Read[i][3] - File.data_Read[0][3]) * 1);
		}
		else if (File.data_Read[i][2] == 4)
		{
			camDData.push_back(File.data_Read[i][0], File.data_Read[i][1], (File.data_Read[i][3] - File.data_Read[0][3])*1);
		}

	}
	//======================================================================
	

	sysClock.reset();
	//sysClock.SetNow(File.data_Read[0][3]);

	cCameraSource CamA("127.0.0.1", 2002, camAData);
	cCameraSource CamB("127.0.0.1", 2003, camBData);
	cCameraSource CamC("127.0.0.1", 2004, camCData);
	cCameraSource CamD("127.0.0.1", 2005, camDData);
	//CamA.CameraThread();

	CamA.CamOn();
	CamB.CamOn();
	CamC.CamOn();
	CamD.CamOn();
	while (shell.getCommand() != 'q' )
	{
		if (cmd == 's'
			&& CamA.state !='s'
			&& CamB.state != 's'
			&& CamC.state != 's'
			&& CamD.state != 's')
		{
			t_start = sysClock.now() + 1000 ;
			cout << t_start << endl;
			CamA.state = 's';
			CamB.state = 's';
			CamC.state = 's';
			CamD.state = 's';
			cmd = '\0';
		}
		
	}
	return 0;
}