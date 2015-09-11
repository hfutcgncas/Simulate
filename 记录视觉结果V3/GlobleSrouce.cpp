#include"GlobleSrouce.h"
#include"math.h"

#include"iostream"

using namespace std;
//globle ver--------------------
cFile  dataFile;
cClock SystemClock;
//-----------------------------------------


#pragma region cClock 全局时钟
cClock::cClock()
{
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

void cClock::reset()
{
	QueryPerformanceCounter(&litmp);
	timeSysStart = litmp.QuadPart;
}

double cClock::now()//单位为ms
{
	double nowRead;
	QueryPerformanceCounter(&litmp);
	timeSysNow = litmp.QuadPart; 
	timeSysNow -= timeSysStart;
	if(timeSysNow<0)timeSysNow += 0x7FFFFFFF;  //LONGLONG 为64位整形，故如此进位
	nowRead    = timeSysNow/dfFreq*1000.0;
	if(nowRead>1000000)
	{
		nowRead -= 1000000;
		this->reset();
	}
	return nowRead;
}
#pragma endregion
//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region cShell 交互命令
char cShell::getCommand()
{
	key = '0';
	if (_kbhit())
	{//有按键
		key = getch();
		switch(key)
		{
		case 'q':
			dataFile.SaveTraceData(0,0,0,0,0,0,0,1);
			printf("\n退出程序.\n");
			break;
		case 's':
			//保存文件,按一次S，保存一次，并清空数据区
			if (!dataFile.m_bStartSave)
			{
				dataFile.m_bStartSave = true;
				printf("Begin to Save File.\n");
			}
			else
			{
				dataFile.m_bStartSave = false;
				dataFile.SaveTraceData(0,0,0,0,0,0,0,1);//关闭文件
				printf("Stop Save File.\n");
			}
			break;
		default:
			printf("\n命令错误.\n");
		}
	}
	return key;

}
#pragma endregion
//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region cFile 文件操作
cFile::cFile()
{
	m_bStartSave  = false ;
	bFileCreate   = false ;
	fileNameIndex = 0     ;
}

void cFile::SaveTraceData(double x,double y,double z,double t,double vx,double vy,double vz,int flag)
{//保存数据
	//flag = 88:保存88的数据 98:保存98的数据,其他：关闭文件
	if (flag==1)
	{//关闭
		if (bFileCreate)
		{
			fclose(fp);
			bFileCreate = false;
			char data[100];
			sprintf(data,"%d",++fileNameIndex);
			WritePrivateProfileString("SaveFilePara","FileIndex",data, "ballData\\sys.ini");
		}
		return;
	}
	if (flag==88)
	{
		if (bFileCreate==false)
		{
			//读取ini文件
			char iniData[100];
			GetPrivateProfileString("SaveFilePara","FileIndex","0",iniData,100,"ballData\\sys.ini");
			fileNameIndex = atoi(iniData);		
			sprintf(fileName,"%s\\ballData%d.txt",DirName,fileNameIndex);
			if((fp=fopen(fileName,"at"))==NULL)
			{//创建
				printf("\nCreate File Failed!\n");
				return;
			}
			bFileCreate = true;
		}
		else
		{//写入
			fprintf(fp,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f\n", x,y,z,t,vx,vy,vz);
		}
	}
}

bool cFile::FileInit()
{
	struct tm *localTime;
	time_t ti;
	char strTime[100];
	strTime[0]='0';
	ti=time(NULL); 
	localTime=localtime(&ti);	//获取日期 
	
	if(_access("ballData",0)==-1)
	{//文件夹不存在
		mkdir("ballData");
	}
	
	sprintf(DirName,"ballData\\球轨迹数据_%04d_%02d_%02d",
		localTime->tm_year+1900,
		localTime->tm_mon+1,
		localTime->tm_mday);
	if (_access(DirName,0)==-1)
	{
		mkdir(DirName); 
		DeleteFile("ballData\\sys.ini");
		//新建文件夹
	}
	return 1;
}
#pragma endregion 
//=================================================================================================
//=================================================================================================
//=================================================================================================
bool cBallModel::predict_OneStep(BallPoint In, BallPoint* pOut, double dt)
{
	//本函数中单位为 m ，m/s ，s
	double V;
	double a[3];
	double C1 = 0.1500, C2 = 0.0060, g = 9.802;

	V = sqrt(In.Vx * In.Vx + In.Vy * In.Vy + In.Vz * In.Vz);
	a[0] = -C1*V*In.Vx + C2*In.Wy*In.Vz - C2*In.Wz*In.Vy;
	a[1] = -C1*V*In.Vy + C2*In.Wz*In.Vx - C2*In.Wx*In.Vz;
	a[2] = -C1*V*In.Vz + C2*In.Wx*In.Vy - C2*In.Wy*In.Vx - g;


	pOut->x = In.x + In.Vx * dt;
	pOut->y = In.y + In.Vy * dt;
	pOut->z = In.z + In.Vz * dt;

	pOut->Vx = In.Vx + a[0] * dt;
	pOut->Vy = In.Vy + a[1] * dt;
	pOut->Vz = In.Vz + a[2] * dt;

	pOut->Wx = In.Wx;
	pOut->Wy = In.Wy;
	pOut->Wz = In.Wz;

	pOut->t = In.t + dt;
	return true;
}
bool cBallModel::predict_Rebound(BallPoint In, BallPoint* pOut)
{
	double r = 0.02;
	double u = 0.25, et = 0.93;  // 需要找到合适的摩擦系数0.25、反弹系数
	//double u = 0.25, et = 0.8;  // 需要找到合适的摩擦系数0.25、反弹系数
	double vbt1 = In.x - r*In.Wy;
	double vbt2 = In.y + r*In.Wx;
	double vs = 1 - 2.5*u*(1 + et)*signFun(In.Vz) / sqrt(vbt1*vbt1 + vbt2*vbt2);
	double aa = u*(1 + et)*signFun(In.Vz) / sqrt(vbt1*vbt1 + vbt2*vbt2);
	if (vs <= 0)
	{
		aa = 2 / 5.0;
	}

	pOut->Vx = (1 - aa)*(In.Vx) + aa*r*In.Wy;
	pOut->Vy = (1 - aa)*(In.Vy) - aa*r*In.Wx;
	pOut->Vz = -et*(In.Vz);


	pOut->Wx = -3.0*aa / 2.0 / r*(In.Vy) + (1 - 3 * aa / 2)*In.Wx;
	pOut->Wy = 3.0*aa / 2.0 / r*(In.Vx) + (1 - 3 * aa / 2)*In.Wy;
	pOut->Wz = In.Wz;

	pOut->x = In.x;
	pOut->y = In.y;
	pOut->z = In.z;

	pOut->t = In.t;

	return true;
}

int cBallModel::predict(BallPoint startPoint, BallPoint* pEndPoint,double* confidence, double stopPlane)
//return 1 : 正常
//return -1 : 反向
//return 2 : 未反弹
//return 3 : 超时
//return 4 : 反弹后还是够不着
{
	int count = 0;
	double dt = 0.0005;
	BallPoint temp1;
	BallPoint temp2;
	temp1.Copy(startPoint);
	if (startPoint.Vx < 0 )
	{
		*confidence = 0;
		return PREDICT_BACKFORWARD;//反向不预测
	}

	//-----------------------------------------------------------
	//反弹前
	while (temp1.z > 0 || temp1.Vz>0)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;
		
		if (temp1.x > stopPlane)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = count;
			return PREDICT_NOREBOND;  //未反弹
		}
		if (count > 0.5 / dt)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}
	}
	//-----------------------------------------------------------
	//反弹
	predict_Rebound(temp1, &temp2);
	temp1.Copy(temp2);
	//-----------------------------------------------------------
	//反弹后
	while (temp1.x < stopPlane)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;

		if (count > 0.5 / dt)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}
			
		if (temp1.z < 0 && temp1.Vz<=0)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = count;
			return PREDICT_TOOSHORT; //一次反弹后没能达到目标位置
		}
			
	}
	(*pEndPoint).Copy(temp1);
	*confidence = count;
	return PREDICT_NORMAL; //正常
}
int cBallModel::predict_dt(BallPoint startPoint, BallPoint* pEndPoint, double* confidence,double T)
{
	int count = 0;
	double dt = 0.0005;
	BallPoint temp1;
	BallPoint temp2;
	temp1.Copy(startPoint);
	if (startPoint.Vx < 0)
	{
		*confidence = 0;
		return PREDICT_BACKFORWARD;//反向不预测
	}

	//-----------------------------------------------------------
	//反弹前
	while (temp1.z > 0 || temp1.Vz>0)
	{
		predict_OneStep(temp1, &temp2, dt);
		temp1.Copy(temp2);
		count++;



		if (count*dt > T)
		{
			(*pEndPoint).Copy(temp1);
			*confidence = count;
			return PREDICT_NORMAL;  //未反弹
		}
		if (count*dt > 0.5)
		{
			*confidence = 0;
			return PREDICT_TIMEOUT; //超时
		}
	}

	//-------
}
//=================================================================================================
//=================================================================================================
//=================================================================================================
void  BallPoint::Copy(BallPoint src)
{
	x = src.x;
	y = src.y;
	z = src.z;
	Vx = src.Vx;
	Vy = src.Vy;
	Vz = src.Vz;
	Wx = src.Wx;
	Wy = src.Wy;
	Wz = src.Wz;
	t = src.t;
}

BallPoint  BallPoint::operator*(double fac)
{
	BallPoint out;
	out.x = x*fac;
	out.y = y*fac;
	out.z = z*fac;
	out.Vx = Vx*fac;
	out.Vy = Vy*fac;
	out.Vz = Vz*fac;
	out.Wx = Wx*fac;
	out.Wy = Wy*fac;
	out.Wz = Wz*fac;
	out.t = t*fac;

	return out;
}
BallPoint BallPoint::operator + (BallPoint &A)
{
	BallPoint out;
	out.x = x + A.x;
	out.y = x + A.y;
	out.z = x + A.z;
	out.Vx = x + A.Vx;
	out.Vy = x + A.Vy;
	out.Vz = x + A.Vz;
	out.Wx = x + A.Wx;
	out.Wy = x + A.Wy;
	out.Wz = x + A.Wz;
	out.t = x + A.t;
	return out;
}
BallPoint BallPoint::operator - (BallPoint &A)
{
	BallPoint out;
	out.x = x - A.x;
	out.y = x - A.y;
	out.z = x - A.z;
	out.Vx = x - A.Vx;
	out.Vy = x - A.Vy;
	out.Vz = x - A.Vz;
	out.Wx = x - A.Wx;
	out.Wy = x - A.Wy;
	out.Wz = x - A.Wz;
	out.t = x - A.t;
	return out;
}

void  BallPoint::DispPoint()
{
	cout << "pos:" << endl;
	cout << x << " " << y << " " << z << " " << t << endl;
}

//=================================================================================================
//=================================================================================================
//=================================================================================================

////==========================================================================================
//乒乓球3D位置队列cHis3DTemp的实现
//==========================================================================================
void cBallPosTemp::push_back(double x, double y, double z, double vx, double vy, double vz, double wx, double wy, double wz, double time)
{
	tempX.push_back(x);
	tempY.push_back(y);
	tempZ.push_back(z);
	tempVx.push_back(vx);
	tempVy.push_back(vy);
	tempVz.push_back(vz);
	tempWx.push_back(wx);
	tempWy.push_back(wy);
	tempWz.push_back(wz);
	tempTime.push_back(time);
}
void cBallPosTemp::push_back(BallPoint newPoint)
{
	tempX.push_back(newPoint.x);
	tempY.push_back(newPoint.y);
	tempZ.push_back(newPoint.z);
	tempVx.push_back(newPoint.Vx);
	tempVy.push_back(newPoint.Vy);
	tempVz.push_back(newPoint.Vz);
	tempWx.push_back(newPoint.Wx);
	tempWy.push_back(newPoint.Wy);
	tempWz.push_back(newPoint.Wz);
	tempTime.push_back(newPoint.t);

}

void cBallPosTemp::clear()
{
	tempX.clear();
	tempY.clear();
	tempZ.clear();
	tempVx.clear();
	tempVy.clear();
	tempVz.clear();
	tempWx.clear();
	tempWy.clear();
	tempWz.clear();
	tempTime.clear();
}
int cBallPosTemp::size()
{
	return (int)(tempX.size());
}
void cBallPosTemp::del()
{
	tempX.erase(tempX.begin());
	tempY.erase(tempY.begin());
	tempZ.erase(tempZ.begin());
	tempVx.erase(tempVx.begin());
	tempVy.erase(tempVy.begin());
	tempVz.erase(tempVz.begin());
	tempWx.erase(tempWx.begin());
	tempWy.erase(tempWy.begin());
	tempWz.erase(tempWz.begin());
	tempTime.erase(tempTime.begin());
}
void cBallPosTemp::copy(cBallPosTemp src)
{
	tempX = src.tempX;
	tempY = src.tempY;
	tempZ = src.tempZ;
	tempVx = src.tempVx;
	tempVy = src.tempVy;
	tempVz = src.tempVz;
	tempWx = src.tempWx;
	tempWy = src.tempWy;
	tempWz = src.tempWz;
	tempTime = src.tempTime;
}


//=================================================================================================
//=================================================================================================
//=================================================================================================
cPrediectOutput::cPrediectOutput()
{
	Output = Output*0;
	HisGroup.Ball_Start = HisGroup.Ball_Start * 0;
	HisGroup.Ball_Stop = HisGroup.Ball_Stop * 0;
	HisGroup.Confidence = 0;
}


void cPrediectOutput::RefreashOutput(PrediectGroup CurrentGroup)
{
	double fac = 0;
	
	if (HisGroup.Confidence == 0)
	{
		HisGroup.Ball_Start = CurrentGroup.Ball_Start;
		HisGroup.Ball_Stop  = CurrentGroup.Ball_Stop;
		HisGroup.Confidence = CurrentGroup.Confidence;
		return;
	}
	cBallModel model;

	BallPoint PredictCurrent;

	double a = CurrentGroup.Confidence;
	double b = HisGroup.Confidence;
	double Dt = CurrentGroup.Ball_Start.t - HisGroup.Ball_Start.t;

	model.predict_dt(HisGroup.Ball_Start, &PredictCurrent, &a, Dt);


	fac = a / sqrt(a*a + b*b);

	PrediectGroup NewGroup;
	NewGroup.Ball_Start = PredictCurrent + (CurrentGroup.Ball_Start - PredictCurrent)*fac;

	HisGroup.Ball_Start = NewGroup.Ball_Start;
	HisGroup.Confidence = a*b / (a*a + b*b);

	Output.Copy(NewGroup.Ball_Start);
		
	//EstimatedBallStatue = EstimatedBallStatue + (InPoint - EstimatedBallStatue)*fac;
	//Output = Output*(1 - fac) + InPoint*fac;
}