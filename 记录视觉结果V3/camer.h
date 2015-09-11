#ifndef _CAMER_H_
#define _CAMER_H_

#include <vector>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

extern "C"
{
	#include <my++\myhead.h>
}


using std::vector;

#define LENTH_OF_RECBUF 20

DWORD WINAPI ThreadProc( LPVOID lpParam );

class cCamData
{
public:
	int x;
	int y;
	double time;
};

class cTempData
{
public:
	vector<double> tempX;
	vector<double> tempY;
	vector<double> tempTime;
	
	void push_back(double x,double y,double time);
	void clear();
	int  size();
	void  del();
	void copy(cTempData src);
};


//========================================//
class cCamer
{
public:
	HANDLE hThread1,hThread2;
	DWORD ThreadID;

	char* IP;
	u_short port;

	SOCKET		sockClient;
	SOCKADDR_IN	addrSrv;
    char recvBuf[LENTH_OF_RECBUF];          // 存放收到的信息
	
	//存放最后一次读取的信息---------------
	double x_cur;
	double y_cur;
	double t_cur;
	int    dataReadFlag;

	//-------------------------------------
	bool busyFlag;


	bool Communicate_2_Cam( char * sendbuf , char * recvbuf );
	char Refreash_Data();
	//-------------------------------------

	bool AskForData();//发送索取信号
	bool GetForData(double SysTime);
	//-------------------------------------

	char cur_index;
	char last_index;
	
	char New_Data_Rcv;

	

	//====================================
	cCamer();
	cCamer(char* IP,u_short port);
	~cCamer();
	//====================================

	char CamNetInit(void);
	void CamOn(void);
	bool cCamer::CamSync();
	char Camthread(void);
	//====================================

	cCamData camData[10];

	int index;

	cTempData FilterTemp;
	cTempData DoubtTemp;
	int FilterState;
	void CamFilter(  );


	Matrix *pz1;
	Matrix *pz2;
	Matrix *pz3;
	Matrix *pz4 ;

	Matrix *pH ;

	Matrix *pgls1;
	Matrix *pgls2;


};

#endif//_CAMER_H_
