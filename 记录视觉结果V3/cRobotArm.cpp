/*���ļ����ڶ��屾PC���˶�����PC���ͨѶ����������
1��MotionNetInit()����ʼ�����˶�����PC����������
2��Cmd2Motion���������˶�����PC����Ԥ��Ļ���㼰����ʱ��
*/

#include "cRobotArm.h"

 SOCKET sockClient_Motion;
 SOCKADDR_IN addrSrv_Motion;

bool cRobotArm::ComInit()
{
	WORD wVersionRequested;	
	WSADATA wsaData;
	int err;	

	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		return 0; 
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{
		WSACleanup( );
		return 0; 
	}	
	sockClient_Motion=socket(AF_INET,SOCK_DGRAM,0);
	//addrSrv_Motion.sin_addr.S_un.S_addr=inet_addr("192.168.64.82");
	addrSrv_Motion.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv_Motion.sin_family = AF_INET;
	addrSrv_Motion.sin_family=AF_INET;
	addrSrv_Motion.sin_port=htons(7000);

	return true;
}

bool cRobotArm::HitBall(double X_hit,double Y_hit,double Z_hit,double hit_vx0,double hit_vy0,double hit_vz0,double t_predict)
{
	double Sendbuff[13];

	Sendbuff[0] =  1.0;
	Sendbuff[1] =  (Y_hit-0.3)*1000; //����������ϵx
	//X_hit�ķ�Χ���в���
	if (X_hit-1.1 < 0.4 )
	{
		Sendbuff[2] =  (-X_hit+1.1+1.1 )*1000; 
	}
	else if (X_hit-1.1 < 0.8 )
	{
		Sendbuff[2] =  (-X_hit+1.1+1.2 )*1000; 
	}
	else 
	{
		Sendbuff[2] =  (-X_hit+1.1+1.3 )*1000; 
	}

	Sendbuff[3] =  Z_hit*1000; //����������ϵz
	Sendbuff[4] =  hit_vy0*1000;//����������ϵx
	Sendbuff[5] =  -hit_vx0*1000;//����������ϵy
	Sendbuff[6] =  hit_vz0*1000;
	Sendbuff[7] =  0;
	Sendbuff[8] =  0; //����ʱ��,�������ʱ�̣����ػ�����������
	Sendbuff[9] =  0;
	Sendbuff[10] = 0;
	Sendbuff[11] = 0;
	Sendbuff[12] = t_predict; //ms



	//for (int k = 0;k<13;k++)
	//{
	//	printf(" %4f",Sendbuff[k]);
	//	printf("\n");
	//}
	

	cout<<"RD 2 send"<<endl;
	//getchar();

	if(sendto(sockClient_Motion,(char *)Sendbuff,sizeof(Sendbuff)*13,0,
		(SOCKADDR*)&addrSrv_Motion,sizeof(SOCKADDR)))
	{
		//printf("����ǰ�������ݳɹ�!\n");
	}
	else
	{
		//printf("����ǰ��������ʧ��!\n");
	}
	return 1;
};