//本文件包括以下内容
/*
1、初始化摄像机参数
2、3D重建乒乓球轨迹的算法实现函数
3、乒乓球3D位置队列cHis3DTemp的实现
4、轨迹预测的函数
*/
#include "Cam3D.h"

#include <iostream>
using namespace std;

Matrix *M88;
Matrix *M89;
Matrix *M98;
Matrix *M99;
extern cClock SystemClock;


//==========================================================================================
//1、初始化摄像机参数
//==========================================================================================
void InitMatrax()//初始化摄像机参数
{
	//==========================================================================================
	//摄像机参数
	//==========================================================================================
	/*double Cam88ExData[16] = { -0.290444202475404,0.946157493277952,-0.142927125355577,-516.529995020899,
								0.514050823500147,0.0282962142234425,-0.857292875929482,-247.233011991509,
								-0.807089781937083,-0.322467552126727,-0.494591509953567,4246.34501554548-50,
								0,0,0,1 };
	double Cam88InData[12] = {1104.15142117353,	0,	276.301391445049,	0,
		                      0,	1098.29837231284,	236.838737980044,	0,
		                      0,	0,	1,	0};

	double Cam89ExData[16] = {   0.459414497498630,0.875817695219114,0.147924589670414,-485.798184588997,
								0.496219544128832,-0.114947846252864,-0.860553982424346,-236.619158159129,
								-0.736684792508068,0.468754047857623,-0.487406564486402,4044.02296250964-50,
								0,0,0,1 };

	double Cam89InData[12] = { 1147.23745220316	,0	,329.238936880755	,0,
								0	,1147.40720489817,	234.612718040265,	0,
								0	,0	,1	,0};

	double Cam98ExData[16] = {  0.186666 ,	 -0.978722 ,	 -0.085200 ,  -751.439824  ,
		                        -0.589527, 	 -0.042216 , -0.806645  , -260.305300,
		                        0.785884 ,	 0.200801 	, -0.584863 ,  3084.210249,
		                        0        ,           0 ,         0     ,     1         };

	double Cam98InData[12] = {1050.77334,   0,           466.65185 ,  0, 
		                        0  ,        1148.63287 , 410.29566 ,  0,
		                        0  ,         0  ,          1   ,      0};


	double Cam99ExData[16] = {  -0.483594 ,	 -0.839359 ,	 0.248220  ,      105.439430,
								-0.498046 ,	 0.030670 ,	 -0.866608     ,  279.535740,
								0.719782 ,	 -0.542711 ,	 -0.432872 ,      3734.464675,
								0	     ,     0       ,          0    ,          1            };

	double Cam99InData[12] = {  1134.74570 ,   0           ,   345.36929  , 0, 
								0          ,   1138.08795  ,   189.67517  , 0,
								0          ,   0           ,   1          , 0};*/

	//==============================================================
	/*double Cam88ExData[16] = { 0.263443496540838, 0.952110362577571, 0.155188213478810, -787.127717465714,
								-0.570447207775731, 0.0240242604145416, 0.820982836636922, -111.845134134889,
								0.777937984206460, -0.304809272142337, 0.549457732992128, 3763.42491628639,
								0, 0, 0, 1 };

	double Cam88InData[12] = { 1119.33657800349, 0, 285.122582344034, 0,
								0, 1118.21934509895, 254.277562154169, 0,
								0, 0, 1, 0 };

	double Cam89ExData[16] = { -0.454493882971186, 0.850390978095721, -0.265085825187197, -468.003069251984,
								-0.547424685722205, -0.0318958856257074, 0.836246892934196, 20.4268704262938,
								0.702681666050652, 0.525183622034765, 0.480021498833881, 3536.12450906042,
								0, 0, 0, 1 };
	double Cam89InData[12] = { 1130.15461685499, 0, 328.905142402980, 0,
								0, 1128.46243690843, 234.321908334105, 0,
								0, 0, 1, 0 };

	double Cam98ExData[16] = { -0.405999205137351, -0.887773477238780, 0.216847638998455, 260.030183191835,
								0.513323799367383, -0.0252225196230161, 0.857824283584290, 228.244752711422,
								-0.756084203267648, 0.459589031217273, 0.465955577232309, 3375.91167724499,
								0, 0, 0, 1 };
	double Cam98InData[12] = { 1128.29264556218, 0, 305.290771119346, 0,
								0, 1127.19871181080, 219.037100981758, 0,
								0, 0, 1, 0 };

	double Cam99ExData[16] = { 0.476148391060183, -0.842894269600767, -0.250623143314795, 64.1942817570228,
								0.526167013607395, 0.0447308805134711, 0.849203993231288, 194.862978110202,
								-0.704578585739216, -0.536216745917804, 0.464801697409816, 3713.71276447928,
								0, 0, 0, 1 };
	double Cam99InData[12] = { 1134.12462687546, 0, 348.171796189448, 0,
								0, 1133.69297150682, 220.479849963420, 0,
								0, 0, 1, 0 };*/
	//==============================================================
	//double Cam88ExData[16] = { -0.263443496540838, 0.952110362577571, -0.155188213478810, -787.127717465714,
	//							0.570447207775731, 0.0240242604145416, -0.820982836636922, -111.845134134889,
	//							-0.777937984206460, -0.304809272142337, -0.549457732992128, 3763.42491628639,
	//							0, 0, 0, 1 };

	//double Cam88InData[12] = { 1119.33657800349, 0, 285.122582344034, 0,
	//							0, 1118.21934509895, 254.277562154169, 0,
	//							0, 0, 1, 0 };

	//double Cam89ExData[16] = { 0.454493882971186, 0.850390978095721, 0.265085825187197, -468.003069251984,
	//							0.547424685722205, -0.0318958856257074, -0.836246892934196, 20.4268704262938,
	//							-0.702681666050652, 0.525183622034765, -0.480021498833881, 3536.12450906042,
	//							0, 0, 0, 1 };
	//double Cam89InData[12] = { 1130.15461685499, 0, 328.905142402980, 0,
	//							0, 1128.46243690843, 234.321908334105, 0,
	//							0, 0, 1, 0 };

	//double Cam98ExData[16] = { 0.405999205137351, -0.887773477238780, -0.216847638998455, 260.030183191835,
	//							-0.513323799367383, -0.0252225196230161, -0.857824283584290, 228.244752711422,
	//							0.756084203267648, 0.459589031217273, -0.465955577232309, 3375.91167724499,
	//							0, 0, 0, 1 };
	//double Cam98InData[12] = { 1128.29264556218, 0, 305.290771119346, 0,
	//							0, 1127.19871181080, 219.037100981758, 0,
	//							0, 0, 1, 0 };

	//double Cam99ExData[16] = { -0.476148391060183, -0.842894269600767, 0.250623143314795, 64.1942817570228,
	//							-0.526167013607395, 0.0447308805134711, -0.849203993231288, 194.862978110202,
	//							0.704578585739216, -0.536216745917804, -0.464801697409816, 3713.71276447928,
	//							0, 0, 0, 1 };
	//double Cam99InData[12] = { 1134.12462687546, 0, 348.171796189448, 0,
	//							0, 1133.69297150682, 220.479849963420, 0,
	//							0, 0, 1, 0 };

	////==============================================================
//	//2015-5-11
//double Cam88ExData[16] = { -0.257884652105813, 0.953943037945654, -0.153258561141882, -787.009352094627,
//0.566318395047146, 0.0207256833271414, -0.823925919899264, -111.662050449976,
//-0.782802006665471, -0.299270991587290, -0.545580325850257, 3764.63900178184,
//0, 0, 0, 1 };
//double Cam88InData[12] = { 1119.33657800349, 0, 285.122582344034, 0,
//0, 1118.21934509895, 254.277562154169, 0,
//0, 0, 1, 0 };
//
//double Cam89ExData[16] = { 0.456046042261304, 0.849540578597730, 0.265146775680255, -467.867789769649,
//0.548216296534328, -0.0334741088819582, -0.835666426421898, 20.4121750536892,
//-0.701056987378335, 0.526460149821801, -0.480997724628319, 3536.13353857092,
//0, 0, 0, 1 };
//double Cam89InData[12] = { 1130.15461685499, 0, 328.905142402980, 0,
//0, 1128.46243690843, 234.321908334105, 0,
//0, 0, 1, 0 };
//
//double Cam98ExData[16] = { 0.409040524924938, -0.885586619650985, -0.220050421640752, 260.003857875815,
//-0.513563765279132, -0.0240836997166941, -0.857713375435103, 228.294907470186,
//0.754279860503681, 0.463849452412200, -0.464656408043030, 3372.96720147832,
//0, 0, 0, 1 };
//double Cam98InData[12] = { 1128.29264556218, 0, 305.290771119346, 0,
//0, 1127.19871181080, 219.037100981758, 0,
//0, 0, 1, 0 };
//
//double Cam99ExData[16] = { -0.474315927406530, -0.844568598504174, 0.248451772843704, 64.5062933751489,
//-0.525138639132591, 0.0449220989160976, -0.849830226997689, 194.778663602010,
//0.706578948666357, -0.533559638137733, -0.464822871481038, 3713.67129159282,
//0, 0, 0, 1 };
//double Cam99InData[12] = { 1134.12462687546, 0, 348.171796189448, 0,
//0, 1133.69297150682, 220.479849963420, 0,
//0, 0, 1, 0 };
	//==============================================================
//2015-6-2
double Cam88ExData[16] = { -0.257884652105813,0.953943037945654,-0.153258561141882,-548.523592608214,
							0.566318395047146,0.0207256833271414, -0.823925919899264,-106.480629618191,
							- 0.782802006665471,-0.299270991587290,-0.545580325850257,3689.82125388502,
							0,0,0,1 };
double Cam88InData[12] = { 1119.33657800349, 0, 285.122582344034, 0,
							0, 1118.21934509895, 254.277562154169, 0,
							0, 0, 1, 0 };

double Cam89ExData[16] = { 0.456046042261304,0.849540578597730,0.265146775680255,-255.482645120217,
							0.548216296534328,-0.0334741088819582,-0.835666426421898,12.0436478331997,
							- 0.701056987378335,0.526460149821801,-0.480997724628319,3667.74857602637,
							0,0,0,1 };
double Cam89InData[12] = { 1130.15461685499, 0, 328.905142402980, 0,
							0, 1128.46243690843, 234.321908334105, 0,
							0, 0, 1, 0 };

double Cam98ExData[16] = { 0.409040524924938,-0.885586619650985,-0.220050421640752,38.6072029630688,
							- 0.513563765279132,-0.0240836997166941,-0.857713375435103,222.273982541012,
							0.754279860503681,0.463849452412200,-0.464656408043030,3488.92956458137,
							0,0,0,1 };
double Cam98InData[12] = { 1128.29264556218, 0, 305.290771119346, 0,
							0, 1127.19871181080, 219.037100981758, 0,
							0, 0, 1, 0 };

double Cam99ExData[16] = { -0.474315927406530,-0.844568598504174,0.248451772843704,-146.635856250895,
							- 0.525138639132591,0.0449220989160976,-0.849830226997689,206.009188331034,
							0.706578948666357,-0.533559638137733,-0.464822871481038,3580.28138205839,
							0,0,0,1 };
double Cam99InData[12] = { 1134.12462687546, 0, 348.171796189448, 0,
							0, 1133.69297150682, 220.479849963420, 0,
							0, 0, 1, 0 };
//==============================================================


	Matrix *Cam89Ex;
	Matrix *Cam88Ex;
	Matrix *Cam89In;
	Matrix *Cam88In;
	Matrix *Cam98Ex;
	Matrix *Cam98In;
	Matrix *Cam99Ex;
	Matrix *Cam99In;


	Cam89Ex = toMatrix(Cam89ExData,4,4);
	Cam88Ex = toMatrix(Cam88ExData,4,4);
	Cam98Ex = toMatrix(Cam98ExData,4,4);
	Cam99Ex = toMatrix(Cam99ExData,4,4);

	Cam89In = toMatrix(Cam89InData,3,4);
	Cam88In = toMatrix(Cam88InData,3,4);
	Cam98In = toMatrix(Cam98InData,3,4);
	Cam99In = toMatrix(Cam99InData,3,4);

	M88 = MdotM(Cam88In,Cam88Ex);
	M89 = MdotM(Cam89In,Cam89Ex);
	M98 = MdotM(Cam98In,Cam98Ex);
	M99 = MdotM(Cam99In,Cam99Ex);

	freeMatrix(Cam89Ex);
	freeMatrix(Cam88Ex);
	freeMatrix(Cam98Ex);
	freeMatrix(Cam99Ex);
	freeMatrix(Cam89In);
	freeMatrix(Cam88In);
	freeMatrix(Cam98In);
	freeMatrix(Cam99In);
}
//==========================================================================================
//2、3D重建乒乓球轨迹的算法实现函数
//==========================================================================================
Matrix * Cam3D(Matrix* MA, cTempData FilterTempA,Matrix* MB,cTempData FilterTempB)//用论文中的方法,通过最小二乘来重建3维轨迹
{
	int i,j;
	double in_temp1,in_temp2,in_temp3,in_temp4;
	double t;
	double u,v;

	Matrix* Hl  = newMatrix(2*FilterTempA.size() + 2*FilterTempB.size(),6);
	Matrix* Zl  = newMatrix(2*FilterTempA.size() + 2*FilterTempB.size(),1);
	Matrix* gls = newMatrix(6,1);
	for(i = 0;i< FilterTempA.size();i++)
	{
		u = FilterTempA.tempX[i];
		v = FilterTempA.tempY[i];
		t = FilterTempA.tempTime[i];
		//------------------
		in_temp1 = getMatrix(MA,0,0) - u * getMatrix(MA,2,0);
		in_temp2 = getMatrix(MA,0,1) - u * getMatrix(MA,2,1);
		in_temp3 = getMatrix(MA,0,2) - u * getMatrix(MA,2,2);
		in_temp4 = getMatrix(MA,2,3) * u - getMatrix(MA,0,3);

		setMatrix(Hl, 2*i ,0 , in_temp1);
		setMatrix(Hl, 2*i ,1 , in_temp1*t);
		setMatrix(Hl, 2*i ,2 , in_temp2);
		setMatrix(Hl, 2*i ,3 , in_temp2*t);
		setMatrix(Hl, 2*i ,4 , in_temp3);
		setMatrix(Hl, 2*i ,5 , in_temp3*t);

		setMatrix(Zl, 2*i ,0 , in_temp4);
		//------------------
		in_temp1 = getMatrix(MA,1,0) - v * getMatrix(MA,2,0);
		in_temp2 = getMatrix(MA,1,1) - v * getMatrix(MA,2,1);
		in_temp3 = getMatrix(MA,1,2) - v * getMatrix(MA,2,2);
		in_temp4 = getMatrix(MA,2,3) * v - getMatrix(MA,1,3);

		setMatrix(Hl, 2*i+1 ,0 , in_temp1);
		setMatrix(Hl, 2*i+1 ,1 , in_temp1*t);
		setMatrix(Hl, 2*i+1 ,2 , in_temp2);
		setMatrix(Hl, 2*i+1 ,3 , in_temp2*t);
		setMatrix(Hl, 2*i+1 ,4 , in_temp3);
		setMatrix(Hl, 2*i+1 ,5 , in_temp3*t);

		setMatrix(Zl, 2*i+1 ,0 , in_temp4);
		//------------------//------------------//------------------//
	}

	j = 2* FilterTempA.size();
	for(i = 0;i< FilterTempB.size();i++)
	{
		u = FilterTempB.tempX[i];
		v = FilterTempB.tempY[i];
		t = FilterTempB.tempTime[i];
		//------------------
		in_temp1 = getMatrix(MB,0,0) - u * getMatrix(MB,2,0);
		in_temp2 = getMatrix(MB,0,1) - u * getMatrix(MB,2,1);
		in_temp3 = getMatrix(MB,0,2) - u * getMatrix(MB,2,2);
		in_temp4 = getMatrix(MB,2,3) * u - getMatrix(MB,0,3);

		setMatrix(Hl, 2*i +j,0 , in_temp1);
		setMatrix(Hl, 2*i +j,1 , in_temp1*t);
		setMatrix(Hl, 2*i +j,2 , in_temp2);
		setMatrix(Hl, 2*i +j,3 , in_temp2*t);
		setMatrix(Hl, 2*i +j,4 , in_temp3);
		setMatrix(Hl, 2*i +j,5 , in_temp3*t);

		setMatrix(Zl, 2*i +j,0 , in_temp4);
		//------------------
		in_temp1 = getMatrix(MB,1,0) - v * getMatrix(MB,2,0);
		in_temp2 = getMatrix(MB,1,1) - v * getMatrix(MB,2,1);
		in_temp3 = getMatrix(MB,1,2) - v * getMatrix(MB,2,2);
		in_temp4 = getMatrix(MB,2,3) * v - getMatrix(MB,1,3);

		setMatrix(Hl, 2*i+1+j ,0 , in_temp1);
		setMatrix(Hl, 2*i+1+j ,1 , in_temp1*t);
		setMatrix(Hl, 2*i+1+j ,2 , in_temp2);
		setMatrix(Hl, 2*i+1+j ,3 , in_temp2*t);
		setMatrix(Hl, 2*i+1+j ,4 , in_temp3);
		setMatrix(Hl, 2*i+1+j ,5 , in_temp3*t);

		setMatrix(Zl, 2*i+1+j ,0 , in_temp4);
	}
	//------------------//------------------//------------------//
	//------------------//------------------//------------------//
	//------------------//------------------//------------------//
	//gls =MdotM((pinvMatrix(Hl)),Zl);
	gls = MdotM( MdotM(invMatrix(MdotM(transposeMatrix(Hl),Hl)),transposeMatrix(Hl)), Zl );
	
	freeMatrix(Hl);
	freeMatrix(Zl);
	
	return gls;
}


Matrix * Cam3D(Matrix* MA, cTempData FilterTempA,Matrix* MB,cTempData FilterTempB,Matrix* MC, cTempData FilterTempC)
{
	int i,j,m;
	double in_temp1,in_temp2,in_temp3,in_temp4;
	double t;
	double u,v;

	Matrix* Hl  = newMatrix(2*FilterTempA.size() + 2*FilterTempB.size() + 2*FilterTempC.size() ,6);
	Matrix* Zl  = newMatrix(2*FilterTempA.size() + 2*FilterTempB.size() + 2*FilterTempC.size() ,1);
	Matrix* gls = newMatrix(6,1);

	for(i = 0;i< FilterTempA.size();i++)
	{
		u = FilterTempA.tempX[i];
		v = FilterTempA.tempY[i];
		t = FilterTempA.tempTime[i];
		//------------------
		in_temp1 = getMatrix(MA,0,0) - u * getMatrix(MA,2,0);
		in_temp2 = getMatrix(MA,0,1) - u * getMatrix(MA,2,1);
		in_temp3 = getMatrix(MA,0,2) - u * getMatrix(MA,2,2);
		in_temp4 = getMatrix(MA,2,3) * u - getMatrix(MA,0,3);

		setMatrix(Hl, 2*i ,0 , in_temp1);
		setMatrix(Hl, 2*i ,1 , in_temp1*t);
		setMatrix(Hl, 2*i ,2 , in_temp2);
		setMatrix(Hl, 2*i ,3 , in_temp2*t);
		setMatrix(Hl, 2*i ,4 , in_temp3);
		setMatrix(Hl, 2*i ,5 , in_temp3*t);

		setMatrix(Zl, 2*i ,0 , in_temp4);
		//------------------
		in_temp1 = getMatrix(MA,1,0) - v * getMatrix(MA,2,0);
		in_temp2 = getMatrix(MA,1,1) - v * getMatrix(MA,2,1);
		in_temp3 = getMatrix(MA,1,2) - v * getMatrix(MA,2,2);
		in_temp4 = getMatrix(MA,2,3) * v - getMatrix(MA,1,3);

		setMatrix(Hl, 2*i+1 ,0 , in_temp1);
		setMatrix(Hl, 2*i+1 ,1 , in_temp1*t);
		setMatrix(Hl, 2*i+1 ,2 , in_temp2);
		setMatrix(Hl, 2*i+1 ,3 , in_temp2*t);
		setMatrix(Hl, 2*i+1 ,4 , in_temp3);
		setMatrix(Hl, 2*i+1 ,5 , in_temp3*t);

		setMatrix(Zl, 2*i+1 ,0 , in_temp4);
		//------------------//------------------//------------------//
	}
	j = 2* FilterTempA.size();
	for(i = 0;i< FilterTempB.size();i++)
	{
		u = FilterTempB.tempX[i];
		v = FilterTempB.tempY[i];
		t = FilterTempB.tempTime[i];
		//------------------
		in_temp1 = getMatrix(MB,0,0) - u * getMatrix(MB,2,0);
		in_temp2 = getMatrix(MB,0,1) - u * getMatrix(MB,2,1);
		in_temp3 = getMatrix(MB,0,2) - u * getMatrix(MB,2,2);
		in_temp4 = getMatrix(MB,2,3) * u - getMatrix(MB,0,3);

		setMatrix(Hl, 2*i +j ,0 , in_temp1);
		setMatrix(Hl, 2*i +j,1 , in_temp1*t);
		setMatrix(Hl, 2*i +j,2 , in_temp2);
		setMatrix(Hl, 2*i +j,3 , in_temp2*t);
		setMatrix(Hl, 2*i +j,4 , in_temp3);
		setMatrix(Hl, 2*i +j,5 , in_temp3*t);

		setMatrix(Zl, 2*i +j ,0 , in_temp4);
		//------------------
		in_temp1 = getMatrix(MB,1,0) - v * getMatrix(MB,2,0);
		in_temp2 = getMatrix(MB,1,1) - v * getMatrix(MB,2,1);
		in_temp3 = getMatrix(MB,1,2) - v * getMatrix(MB,2,2);
		in_temp4 = getMatrix(MB,2,3) * v - getMatrix(MB,1,3);

		setMatrix(Hl, 2*i+1+j ,0 , in_temp1);
		setMatrix(Hl, 2*i+1+j ,1 , in_temp1*t);
		setMatrix(Hl, 2*i+1+j ,2 , in_temp2);
		setMatrix(Hl, 2*i+1+j ,3 , in_temp2*t);
		setMatrix(Hl, 2*i+1+j ,4 , in_temp3);
		setMatrix(Hl, 2*i+1+j ,5 , in_temp3*t);

		setMatrix(Zl, 2*i+1+j ,0 , in_temp4);
	}
	j = 2* FilterTempA.size() + 2* FilterTempB.size();
	for(i = 0;i< FilterTempC.size();i++)
	{
		u = FilterTempC.tempX[i];
		v = FilterTempC.tempY[i];
		t = FilterTempC.tempTime[i];
		//------------------
		in_temp1 = getMatrix(MC,0,0) - u * getMatrix(MC,2,0);
		in_temp2 = getMatrix(MC,0,1) - u * getMatrix(MC,2,1);
		in_temp3 = getMatrix(MC,0,2) - u * getMatrix(MC,2,2);
		in_temp4 = getMatrix(MC,2,3) * u - getMatrix(MC,0,3);

		setMatrix(Hl, 2*i +j ,0 , in_temp1);
		setMatrix(Hl, 2*i +j,1 , in_temp1*t);
		setMatrix(Hl, 2*i +j,2 , in_temp2);
		setMatrix(Hl, 2*i +j,3 , in_temp2*t);
		setMatrix(Hl, 2*i +j,4 , in_temp3);
		setMatrix(Hl, 2*i +j,5 , in_temp3*t);

		setMatrix(Zl, 2*i +j ,0 , in_temp4);
		//------------------
		in_temp1 = getMatrix(MC,1,0) - v * getMatrix(MC,2,0);
		in_temp2 = getMatrix(MC,1,1) - v * getMatrix(MC,2,1);
		in_temp3 = getMatrix(MC,1,2) - v * getMatrix(MC,2,2);
		in_temp4 = getMatrix(MC,2,3) * v - getMatrix(MC,1,3);

		setMatrix(Hl, 2*i+1+j ,0 , in_temp1);
		setMatrix(Hl, 2*i+1+j ,1 , in_temp1*t);
		setMatrix(Hl, 2*i+1+j ,2 , in_temp2);
		setMatrix(Hl, 2*i+1+j ,3 , in_temp2*t);
		setMatrix(Hl, 2*i+1+j ,4 , in_temp3);
		setMatrix(Hl, 2*i+1+j ,5 , in_temp3*t);

		setMatrix(Zl, 2*i+1+j ,0 , in_temp4);
	}


	gls = MdotM( MdotM(invMatrix(MdotM(transposeMatrix(Hl),Hl)),transposeMatrix(Hl)), Zl );

	freeMatrix(Hl);
	freeMatrix(Zl);

	return gls;
}


//==========================================================================================
//3、乒乓球3D位置队列cHis3DTemp的实现
//==========================================================================================
void cHis3DTemp::push_back(double x,double y,double z,double vx,double vy,double vz,double time)
{
	tempX.push_back(x);
	tempY.push_back(y);
	tempZ.push_back(z);
	tempVx.push_back(vx);
	tempVy.push_back(vy);
	tempVz.push_back(vz);
	tempTime.push_back(time);
}
void cHis3DTemp::clear()
{
	tempX.clear();
	tempY.clear();
	tempZ.clear();
	tempVx.clear();
	tempVy.clear();
	tempVz.clear();
	tempTime.clear();
}
int cHis3DTemp::size()
{
	return (int)(tempX.size());
}
void cHis3DTemp::del()
{
	tempX.erase(tempX.begin());
	tempY.erase(tempY.begin());
	tempZ.erase(tempZ.begin());
	tempVx.erase(tempVx.begin());
	tempVy.erase(tempVy.begin());
	tempVz.erase(tempVz.begin());
	tempTime.erase(tempTime.begin());
}
void cHis3DTemp::copy(cHis3DTemp src)
{
	tempX = src.tempX;
	tempY = src.tempY;
	tempZ = src.tempZ;
	tempVx = src.tempVx;
	tempVy = src.tempVy;
	tempVz = src.tempVz;
	tempTime = src.tempTime;
}

//==========================================================================================
//4、轨迹预测的函数
//==========================================================================================

double predictX(double x0,double y0,double z0,double Vx0,double Vy0,double Vz0,
			   double *X_land,double * Y_land,double * Z_land,
			   double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
			   double *X_hit,double * Y_hit,double *Z_hit,
			   double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane)
{//单位m
	vector<double> a1(7),a2(7),a3(7);
	double lastTime=0;
	double K=0.12,g=9.802,delt_t=0.001,m=0.0027,Kf=0.000324;
		
	double Kx= 0.648818605362845;
	double bx =0.010490637979372;
	double Ky = 0.643550093254599;
	double by = 0.024063532048931;
	double Kz= -0.707391598175298;
	double bz = 0.468109878762424;

	double temp = 0;
	
	a2[1]=x0;
	a2[2]=y0;
	a2[3]=z0;
	a2[4]=Vx0;
	a2[5]=Vy0;
	a2[6]=Vz0;
	
	a3[1]=a2[4]; 
	a3[2]=a2[5];
	a3[3]=a2[6];
	temp = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+ a2[6]*a2[6]);
	a3[4]=-K*temp*signFun(a2[4]);
	a3[5]=-K*temp*signFun(a2[5]);
	a3[6]=-K*temp*signFun(a2[6])-g;
	
	//physical model prediction method
	while (a2[3]>=0.022 && a2[1] <= stopPlane)
	{
		lastTime += delt_t;
		//----------------------------
		for(int j = 0;j<7;j++)
		{
			a3[j]*=delt_t;
			a2[j]+=a3[j]; 
		}
	//	a3*=delt_t;
	//	a1.Plus(a2,a3);
	//	a2=a1;
		//----------------------------
		a3[1]=a2[4]; 
		a3[2]=a2[5];
		a3[3]=a2[6];
// 		a3[4]=-K*a2[4]*a2[4];
// 		a3[5]=-K*a2[5]*a2[5];
// 		a3[6]=-K*a2[6]*a2[6]-g;
		temp = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+ a2[6]*a2[6]);
		a3[4]=-K*temp*signFun(a2[4]);
		a3[5]=-K*temp*signFun(a2[5]);
		a3[6]=-K*temp*signFun(a2[6])-g;

        delt_t=0.001; 
		//落点附近变步长,使X,z的变化量控制在0.1mm左右。
		if( a2[3]>-0.10 && a2[3]<0.10 )      delt_t=0.00005; 
             
	}
	
	*X_land=a2[1];
	*Y_land=a2[2];	
	*Z_land=a2[3];
	*land_vx0=a2[4];//反弹前的速度
	*land_vy0=a2[5];
	*land_vz0=a2[6];
	*land_preTime = lastTime;
	
	if (a2[1] > stopPlane)
	{
		*X_hit=a2[1];
		*Y_hit=a2[2];
		*Z_hit=a2[3];
		*hit_vx0=a2[4];
		*hit_vy0=a2[5];
		*hit_vz0=a2[6];
		return lastTime;
	}
		
	a2[4]=*land_vx0*Kx+bx;
	a2[5]=*land_vy0*Ky+by;
	a2[6]=*land_vz0*Kz+bz;

	//=====================反弹模型
/*	a2[4] *= 0.98;
	a2[5] *= 0.5;
	a2[6] *= 0.95;
	*/
	a2[4] *= 0.98;
	a2[5] *= 0.98;
	a2[6] *= 0.95;

	//=====================
	
	*land_vx0 = a2[4];//反弹后的速度
	*land_vy0 = a2[5];
	*land_vz0 = a2[6];
	
	a3[1]=a2[4]; 
	a3[2]=a2[5];
	a3[3]=a2[6];
	a3[4]=-K*a2[4]*a2[4];
	a3[5]=-K*a2[5]*a2[5];
	a3[6]=-K*a2[6]*a2[6]-g;
	//此处设置环循结束的条件,
	while (a2[1]<=stopPlane&&a2[3]>-0.050)
//	while (a2[3]<=zstop&&a2[3]>-0.050)
	{
		lastTime += delt_t;
//----------------------------
		for(int j = 0;j<7;j++)
		{
			a3[j]*=delt_t;
			a2[j]+=a3[j]; 
		}
	//	a3*=delt_t;
	//	a1.Plus(a2,a3);
	//	a2=a1;
//----------------------------		
		a3[1]=a2[4];
		a3[2]=a2[5];
		a3[3]=a2[6];
		
		temp = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+ a2[6]*a2[6]);
		a3[4]=-K*temp*signFun(a2[4]);
		a3[5]=-K*temp*signFun(a2[5]);
		a3[6]=-K*temp*signFun(a2[6])-g;

		delt_t=0.001; 
		//落点附近变步长,使X,z的变化量控制在0.1mm左右。
		if( (a2[1]>stopPlane-0.10) && (a2[1]<stopPlane) )             delt_t=0.00005; 
        
	}

	if (a2[1]>stopPlane)
	{
		*X_hit=a2[1];
		*Y_hit=a2[2];
		*Z_hit=a2[3];
		*hit_vx0=a2[4];
		*hit_vy0=a2[5];
		*hit_vz0=a2[6];

		if (*Y_hit<0.1)
		{
			*Y_hit -= 0.05;
			*Z_hit = *Z_hit-0.02;
		}
		if (*Y_hit<0.2)
		{
			*Y_hit -= 0.03;
			*Z_hit = *Z_hit-0.02;
		}
		else if (*Y_hit <0.3)
		{
			*Y_hit += 0.03;
			*Z_hit = *Z_hit-0.03;
		}
		else if (*Y_hit <0.4)
		{
			*Z_hit = *Z_hit-0.02;
			*Y_hit += 0.04;
		}
		else 
		{
			*Z_hit = *Z_hit-0.02;
			*Y_hit += 0.05;
		}

		return lastTime;
	}
	if (a2[1]<=stopPlane)
	{
		*X_hit=0;
		*Y_hit=0;
		*Z_hit=0;
		*hit_vx0=0;
		*hit_vy0=0;
		*hit_vz0=0;
		return 0;
	}
	return lastTime;
}

inline double signFun(double a)
{
	if (a==0)
	{
		return 0.0;
	}
	if (a> 0)
	{
		return a;
	}
	else 
		return -a;
}




double Predict_trace(cHis3DTemp ballPosHis,
			   double *X_land,double * Y_land,double * Z_land,
			   double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
			   double *X_hit,double * Y_hit,double *Z_hit,
			   double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane)
{
	int i;
	double X_land_list[10];double Y_land_list[10];double Z_land_list[10];
	double land_vx0_list[10];double land_vy0_list[10];double land_vz0_list[10];
	double land_preTime_list[10];

	double X_hit_list[10];double Y_hit_list[10];double Z_hit_list[10];
	double hit_vx0_list[10];double hit_vy0_list[10];double hit_vz0_list[10];

	double t_predict_list[10];
	double t_predict = 0;

	
	int count ,tempSize ;
	cout<<"==============================="<<endl;
	tempSize  = ballPosHis.size();
	for(i = 0;i<tempSize;i++)
	{
		
		t_predict_list[i] = predictX( ballPosHis.tempX[i]/1000,ballPosHis.tempY[i]/1000,ballPosHis.tempZ[i]/1000,
							ballPosHis.tempVx[i]/1000,ballPosHis.tempVy[i]/1000,ballPosHis.tempVz[i]/1000,
							&X_land_list[i],&Y_land_list[i],&Z_land_list[i],&land_vx0_list[i],&land_vy0_list[i],&land_vz0_list[i],&land_preTime_list[i],
							&X_hit_list[i], &Y_hit_list[i], &Z_hit_list[i],&hit_vx0_list[i],&hit_vy0_list[i],&hit_vz0_list[i], stopPlane);
	//	t_predict_list[i] = t_predict_list[i]*1000 - (ballPosHis.tempTime[ballPosHis.size()-1] - ballPosHis.tempTime[i]);
		t_predict_list[i] = (t_predict_list[i] + ballPosHis.tempTime[i])*1000;
		//cout<<i<<": "<<X_hit_list[i]<<" "<<Y_hit_list[i]<<" "<<Z_hit_list[i]<<" "<<hit_vx0_list[i]<<" "<<hit_vy0_list[i]<<" "<<hit_vz0_list[i]<<" "<<t_predict_list[i]<<endl;
		
	}
	cout<<"==============================="<<endl;
	*X_hit  = 0;
	*Y_hit = 0;
	*Z_hit = 0;
	*hit_vx0 = 0;
	*hit_vy0 = 0;
	*hit_vz0 = 0;

	count = 0;
	for(i = 0;i<tempSize;i++)
	{
		if(X_hit_list[i]==0)continue;
		count++;
		*X_hit += X_hit_list[i];
		*Y_hit += Y_hit_list[i];
		*Z_hit += Z_hit_list[i];
		*hit_vx0 += hit_vx0_list[i];
		*hit_vy0 += hit_vy0_list[i];
		*hit_vz0 += hit_vz0_list[i];
		t_predict += t_predict_list[i];
		
	}

	if(count == 0)return 0;

		*X_hit  /= count;
		*Y_hit  /= count;
		*Z_hit  /= count;
		*hit_vx0  /= count;
		*hit_vy0  /= count;
		*hit_vz0  /= count;
		t_predict /= count;
		//t_predict -= 80;
		

	return t_predict;

}
//===============================================================================================

void predict_TR(double x0,double y0,double z0,double Vx0,double Vy0,double Vz0,
	double & X_t,double & Y_t,double & Z_t,double &Vx_t,double &Vy_t,double &Vz_t,double &t_t,double x_target)
{
	//此函数中采集的单位为m
	double a1[7],a2[7],a3[7];
	double time_No=0;
	int count=0;
	double K=0.15,g=9.802,delt_t=0.0001;

	a2[1]=x0/1000;
	a2[2]=y0/1000;
	a2[3]=z0/1000;
	a2[4]=Vx0/1000;
	a2[5]=Vy0/1000;
	a2[6]=Vz0/1000;
	x_target /= 1000;

	a3[1]=a2[4]; 
	a3[2]=a2[5];
	a3[3]=a2[6];
	double temp = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+a2[6]*a2[6]);
	a3[4]=-K*temp*a2[4];
	a3[5]=-K*temp*a2[5];
	a3[6]=-K*temp*a2[6]-g;

	//while (delt_t*time_No<=t_end && count < 2000)
	while (   (a2[1] < x_target) 
		   // &&(delt_t*time_No<=t_end)  
		    && count < 2000 
			&& a2[3] > 0.05)
	{

		for (int i = 0;i<7;i++)
		{
			a3[i]*=delt_t;
			a1[i]=a2[i]+a3[i];
			a2[i]=a1[i];
		}
		
		a3[1]=a2[4]; 
		a3[2]=a2[5];
		a3[3]=a2[6];
		temp = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+a2[6]*a2[6]);
		a3[4]=-K*temp*a2[4];
		a3[5]=-K*temp*a2[5];
		a3[6]=-K*temp*a2[6]-g;
		time_No++;
		count++;
	}

	X_t=a2[1]*1000;
	Y_t=a2[2]*1000;	
	Z_t=a2[3]*1000;
	Vx_t = a2[4]*1000;
	Vy_t = a2[5]*1000;
	Vz_t = a2[6]*1000;
	t_t = time_No*delt_t;
}

double Predict_Model(  double *pballStart, 
	double *X_land,double *Y_land,double *Z_land,double *land_Time,
	double *land_vx0,double *land_vy0,double *land_vz0, double *land_vxr,double *land_vyr,double *land_vzr, 
	double land_wx0,double land_wy0,double land_wz0, double *land_wxr,double *land_wyr,double *land_wzr,						
	double *X_hit, double *Y_hit, double *Z_hit, double *hit_vx0, double *hit_vy0, double *hit_vz0,
	double stopPlane)
/*
double Predict_Model(  double x0, double y0, double z0, double Vx0, double Vy0, double Vz0, 
	double *X_land,double *Y_land,double *Z_land,double *land_Time,
	double *land_vx0,double *land_vy0,double *land_vz0, double *land_vxr,double *land_vyr,double *land_vzr, 
	double land_wx0,double land_wy0,double land_wz0, double *land_wxr,double *land_wyr,double *land_wzr,						
	double *X_hit, double *Y_hit, double *Z_hit, double *hit_vx0, double *hit_vy0, double *hit_vz0,
	double stopPlane)*/
{
	double a1[7],a2[7],a3[7];
	double C1 =0.1500, C2 =0.0060, Ts=0.0005, g=9.802, r=0.02;
	double time=0, V = 0;
	int i;

	for (i=0;i<6;i++)
	{
		a2[i+1] = pballStart[i];

	}
	//a2[1]=x0 ;    a2[2]=y0;      a2[3]=z0;
	//a2[4]=Vx0;    a2[5]=Vy0;     a2[6]=Vz0;

	a3[1]=a2[4];  a3[2]=a2[5];   a3[3]=a2[6];

	V = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+a2[6]*a2[6]);
	a3[4]=-C1*V*a2[4]+C2*land_wy0*a2[6]-C2*land_wz0*a2[5];
	a3[5]=-C1*V*a2[5]+C2*land_wz0*a2[4]-C2*land_wx0*a2[6];
	a3[6]=-C1*V*a2[6]+C2*land_wx0*a2[5]-C2*land_wy0*a2[4]-g;

	//飞行模型
	while (a2[3]>0.02 && a2[1] < stopPlane)
	{
		time += Ts;

		for(int j = 0;j<7;j++)
		{
			a3[j]*=Ts;
			a1[j]=a2[j]+a3[j]; 
			a2[j] = a1[j];
		}

		a3[1]=a2[4]; 
		a3[2]=a2[5];
		a3[3]=a2[6];
		V = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+ a2[6]*a2[6]);
		//a3[4]=-C1*V*signFun(a2[4])+C2*land_wy0*signFun(a2[6])-C2*land_wz0*signFun(a2[5]);
		//a3[5]=-C1*V*signFun(a2[5])+C2*land_wz0*signFun(a2[4])-C2*land_wx0*signFun(a2[6]);
		//a3[6]=-C1*V*signFun(a2[6])+C2*land_wx0*signFun(a2[5])-C2*land_wy0*signFun(a2[4])-g;
		a3[4]=-C1*V*a2[4]+C2*land_wy0*a2[6]-C2*land_wz0*a2[5];
		a3[5]=-C1*V*a2[5]+C2*land_wz0*a2[4]-C2*land_wx0*a2[6];
		a3[6]=-C1*V*a2[6]+C2*land_wx0*a2[5]-C2*land_wy0*a2[4]-g;

		//		if( a2[3]>0.02 && a2[3]<0.04 )      Ts=0.0001;
	}

	cout<<"landPos :  ";
	for (i=1;i<7;i++)
	{
		cout<<a2[i]<<" ";
	}
	cout<<endl;

	*X_land=a2[1];      //落点
	*Y_land=a2[2];	
	*Z_land=a2[3];
	*land_vx0=a2[4];	//反弹前的速度
	*land_vy0=a2[5];
	*land_vz0=a2[6];
	*land_Time = time;


	if (a2[1] >= stopPlane)
	{
		*X_hit=a2[1];
		*Y_hit=a2[2];
		*Z_hit=a2[3];
		*hit_vx0=a2[4];
		*hit_vy0=a2[5];
		*hit_vz0=a2[6];
		printf("落点超过击打平面\n");
		return time;
	}

	//反弹模型

	double u=0.25,et=0.93;  // 需要找到合适的摩擦系数0.25、反弹系数
	double vbt1= *land_vx0-r*land_wy0, vbt2= *land_vy0+r*land_wx0;
	double vs=1-2.5*u*(1+et)*signFun(*land_vz0)/sqrt(vbt1*vbt1+vbt2*vbt2);
	double aa=u*(1+et)*signFun(*land_vz0)/sqrt(vbt1*vbt1+vbt2*vbt2);

	if (vs>0)
	{
		*land_vxr=(1-aa)*(*land_vx0)+aa*r*land_wy0;
		*land_vyr=(1-aa)*(*land_vy0)-aa*r*land_wx0;
		*land_vzr=-et*(*land_vz0);

		*land_wxr=-3.0*aa/2.0/r*(*land_vy0)+(1-3*aa/2)*land_wx0;
		*land_wyr=3.0*aa/2.0/r*(*land_vx0)+(1-3*aa/2)*land_wy0;
		*land_wzr=land_wz0;
	}

	if (vs<=0)
	{
		aa=2/5.0;
		*land_vxr=(1-aa)*(*land_vx0)+aa*r*land_wy0;
		*land_vyr=(1-aa)*(*land_vy0)-aa*r*land_wx0;
		*land_vzr=-et*(*land_vz0);

		*land_wxr=-3.0*aa/2/r*(*land_vy0)+(1-3*aa/2)*land_wx0;
		*land_wyr=3.0*aa/2/r*(*land_vx0)+(1-3*aa/2)*land_wy0;
		*land_wzr=land_wz0;
	}


	a2[1]=*X_land;      //落点
	a2[2]=*Y_land;	
	a2[3]=*Z_land;
	a2[4]=*land_vxr;
	a2[5]=*land_vyr;
	a2[6]=*land_vzr;


	a3[1]=a2[4]; 
	a3[2]=a2[5];
	a3[3]=a2[6];
	V = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+a2[6]*a2[6]);

	a3[4]=-C1*V*a2[4]+C2*(*land_wyr)*a2[6]-C2*(*land_wzr)*a2[5];
	a3[5]=-C1*V*a2[5]+C2*(*land_wzr)*a2[4]-C2*(*land_wxr)*a2[6];
	a3[6]=-C1*V*a2[6]+C2*(*land_wxr)*a2[5]-C2*(*land_wyr)*a2[4]-g;
	//stopplane=*X_land+0.25;

	while (a2[1]<stopPlane&&a2[3]>-0.050)
	{
		time += Ts;

		for(int j = 0;j<7;j++)
		{
			a3[j]*=Ts;
			a1[j]=a2[j]+a3[j]; 
			a2[j] = a1[j];
		}

		a3[1]=a2[4]; 
		a3[2]=a2[5];
		a3[3]=a2[6];
		V = sqrt(a2[4]*a2[4]+a2[5]*a2[5]+ a2[6]*a2[6]);
		a3[4]=-C1*V*a2[4]+C2*(*land_wyr)*a2[6]-C2*(*land_wzr)*a2[5];
		a3[5]=-C1*V*a2[5]+C2*(*land_wzr)*a2[4]-C2*(*land_wxr)*a2[6];
		a3[6]=-C1*V*a2[6]+C2*(*land_wxr)*a2[5]-C2*(*land_wyr)*a2[4]-g;

		//		if( (a2[1]>stopplane-0.10) && (a2[1]<stopplane) )            Ts=0.0001;

	}

	if (a2[1]>=stopPlane)
	{
		*X_hit=a2[1];
		*Y_hit=a2[2];
		*Z_hit=a2[3];
		*hit_vx0=a2[4];
		*hit_vy0=a2[5];
		*hit_vz0=a2[6];

		return time;
	}
	if (a2[1]<stopPlane)
	{
		*X_hit=0;
		*Y_hit=0;
		*Z_hit=0;
		*hit_vx0=0;
		*hit_vy0=0;
		*hit_vz0=0;
		printf("未迭代到击打点\n");
		return 0;
	}
	return time;
}

double Predict_traceV2(cHis3DTemp ballPosHis,double* ballHit)
/*	,double *X_land,double * Y_land,double * Z_land,
	double * land_vx0,double* land_vy0,double * land_vz0,double *land_preTime,
	double *X_hit,double * Y_hit,double *Z_hit,
	double* hit_vx0,double* hit_vy0,double * hit_vz0,double stopPlane)*/
{
	int i;
	double testX;
	double testDt;
	int count = ballPosHis.size();
	cHis3DTemp test3D ,testV3D ;
	test3D.copy(ballPosHis);
	double **V;

	V = new double *[count];
	for (i = 0; i< count; i++)V[i] = new double[3];

	testX = ballPosHis.tempX[count-1];

	double avg[7]  ={0,0,0,0,0,0,0};
	double avg2[7] ={0,0,0,0,0,0,0};
	double dlt[7]  ={0,0,0,0,0,0,0};
	for (i = 0; i< count; i++)
	{
		testDt = ballPosHis.tempTime[count-1] - ballPosHis.tempTime[i];
		predict_TR(ballPosHis.tempX[i],ballPosHis.tempY[i],ballPosHis.tempZ[i],ballPosHis.tempVx[i],ballPosHis.tempVy[i],ballPosHis.tempVz[i]
			,test3D.tempX[i],test3D.tempY[i],test3D.tempZ[i],V[i][0],V[i][1],V[i][2],test3D.tempTime[i],testX);
		test3D.tempTime[i] -= testDt;


		avg[0] += test3D.tempX[i];
		avg[1] += test3D.tempY[i];
		avg[2] += test3D.tempZ[i];
		avg[3] += V[i][0];
		avg[4] += V[i][1];
		avg[5] += V[i][2];
		avg[6] += test3D.tempTime[i];

		avg2[0] += test3D.tempX[i]*test3D.tempX[i];
		avg2[1] += test3D.tempY[i]*test3D.tempY[i];
		avg2[2] += test3D.tempZ[i]*test3D.tempZ[i];
		avg2[3] += V[i][0]*V[i][0];
		avg2[4] += V[i][1]*V[i][1];
		avg2[5] += V[i][2]*V[i][2];
		avg2[6] += test3D.tempTime[i]*test3D.tempTime[i];
	}
	
	cout<<"start"<<endl;
	for (i=0;i<7;i++)
	{
		avg[i]/= count;
		avg2[i]/=count;

		avg[i]/= 1000;
		avg2[i]/= 1000;

		dlt[i] = avg2[i] - avg[i]*avg[i];

		cout<<avg[i]<<" ";
	}
	cout<<endl;

	double X_land;
	double Y_land;
	double Z_land;
	double land_vx0;
	double land_vy0;
	double land_vz0;
	double land_vxr;
	double land_vyr;
	double land_vzr;

	double land_wxr;
	double land_wyr;
	double land_wzr;

	double land_Time;

//	double ballHit[7]={0,0,0,0,0,0,0};

	double stopPlane = 2.1;

	double hit_time;



	hit_time = Predict_Model(   avg, 
								&X_land,&Y_land,&Z_land,&land_Time,
								&land_vx0,&land_vy0,&land_vz0, &land_vxr,&land_vyr,&land_vzr, 
								0,0,0,              //w
								&land_wxr,&land_wyr,&land_wzr,						
								&ballHit[0], &ballHit[1], &ballHit[2], &ballHit[3], &ballHit[4], &ballHit[5],
								stopPlane);


	ballHit[6] = hit_time + ballPosHis.tempTime[count-1] - avg[6];
	ballHit[6] -= SystemClock.now()/1000;

	for (i = 0; i< count; i++)delete V[i];
	delete[] V;
	return 0;
}


//========================================
bool cCam3D::Init(char* IP_A, u_short port_A,
	char* IP_B, u_short port_B,
	char* IP_C, u_short port_C,
	char* IP_D, u_short port_D)
{
	CamA.IP = IP_A;
	CamA.port = port_A;
	CamB.IP = IP_B;
	CamB.port = port_B;
	CamC.IP = IP_C;
	CamC.port = port_C;
	CamD.IP = IP_D;
	CamD.port = port_D;


	CamA.CamNetInit();
	CamB.CamNetInit();

	CamC.CamNetInit();
	CamD.CamNetInit();

	Sync();//系统时钟归零
	InitMatrax();//初始化相机参数

	return 1;
}
bool cCam3D::Sync()
{
	CamA.CamSync();
	CamB.CamSync();
	CamC.CamSync();
	CamD.CamSync();

	SystemClock.reset();
	CamA.FilterTemp.clear();
	CamB.FilterTemp.clear();
	CamC.FilterTemp.clear();
	CamD.FilterTemp.clear();

	return 1;
}

extern cFile  dataFile;

char cCam3D::RefreshSrcData()
{
	cTempData TempDataInA,TempDataInB;

	CamA.AskForData();
	CamB.AskForData();
	CamC.AskForData();
	CamD.AskForData();

	bool fa,fb,fc,fd;
	fa = CamA.GetForData(SystemClock.now());
	fb = CamB.GetForData(SystemClock.now());
	fc = CamC.GetForData(SystemClock.now());
	fd = CamD.GetForData(SystemClock.now());	
	
	/*
	if (dataFile.m_bStartSave)
	{
		if(CamA.dataReadFlag)
		{
			dataFile.SaveTraceData(CamA.x_cur * 10, CamA.y_cur * 10, 1, CamA.t_cur, 1, 1, 1, 88);
		}
		if(CamB.dataReadFlag)
		{
			dataFile.SaveTraceData(CamB.x_cur * 10, CamB.y_cur * 10, 2, CamB.t_cur, 2, 1, 1, 88);
		}
		if(CamC.dataReadFlag)
		{
			dataFile.SaveTraceData(CamC.x_cur * 10, CamC.y_cur * 10, 3, CamC.t_cur, 3, 1, 1, 88);
		}
		if(CamD.dataReadFlag)
		{
			dataFile.SaveTraceData(CamD.x_cur * 10, CamD.y_cur * 10, 4, CamD.t_cur, 4, 1, 1, 88);
		}
	}
	*/
	//---------------------------------------------

	if (fa == 1){ContB++;ContA = 0;}//防止有一个摄像头一直收数而另一个收不到数的情况
	if (fb == 1){ContA++;ContB = 0;}

	if (fc == 1){ContD++;ContC = 0;}
	if (fd == 1){ContC++;ContD = 0;}
	if (  ContA < 6 && ContB <6 )
	{
		CamA.CamFilter();
		CamB.CamFilter();		
	}
	if (  ContC < 6 && ContD <6  )
	{
		CamC.CamFilter();
		CamD.CamFilter();
		
	}
	if(CamC.FilterState && CamD.FilterState )return 2;//CD
	if(CamA.FilterState && CamB.FilterState )return 1;//AB
	if (CamA.FilterState && CamC.FilterState)return 3;//AC
	if (CamB.FilterState && CamC.FilterState)return 4;//BC
	if (CamA.FilterState && CamD.FilterState)return 5;//AD
	if (CamB.FilterState && CamD.FilterState)return 6;//BD


	return 0;
}
 
bool cCam3D::ReConstruction(char rt,double* pt,double* px,double* py,double* pz,double* pvx,double* pvy,double* pvz)
{
	Matrix * gls;
	double t_out,x3d,y3d,z3d;

	switch(rt)
	{
	case 1: //AB
		gls = Cam3D(M88,CamA.FilterTemp,M89,CamB.FilterTemp);
		break;
	case 2:  //CD
		gls = Cam3D(M98,CamC.FilterTemp,M99,CamD.FilterTemp);
		//printf("%d  %d\n", CamA.FilterTemp.size(),CamB.FilterTemp.size());    
		break;
	case 3:  //AC
		gls = Cam3D(M88,CamA.FilterTemp,M98,CamC.FilterTemp);
		break;
	case 4:  //BC
		gls = Cam3D(M89,CamB.FilterTemp,M98,CamC.FilterTemp);
		break;
	case 5:  //AD
		gls = Cam3D(M88, CamA.FilterTemp, M99, CamD.FilterTemp);
		break;
	case 6:  //BD
		gls = Cam3D(M89, CamB.FilterTemp, M99, CamD.FilterTemp);
		break;

	default:
		return 0;
	}

	
	t_out = SystemClock.now()/1000 - 0.025;
	x3d = getMatrix(gls,0,0) + getMatrix(gls,1,0)*t_out;
	y3d = getMatrix(gls,2,0) + getMatrix(gls,3,0)*t_out;
	z3d = getMatrix(gls,4,0) + getMatrix(gls,5,0)*t_out;

	//x3d = x3d + 450;  //调节坐标系
	//z3d = z3d + 15;

	*pt = t_out;
	*px = x3d;
	*py = y3d;
	*pz = z3d;

	*pvx = getMatrix(gls,1,0);
	*pvy = getMatrix(gls,3,0);
	*pvz = getMatrix(gls,5,0);

	freeMatrix(gls);
	return 1;
}