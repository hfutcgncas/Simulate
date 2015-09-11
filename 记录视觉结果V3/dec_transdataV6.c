
/***************************************************************************************************************************/
#define NEW_IMAGE_VAR
#include <vcrt.h>
#include <vclib.h>
#include <macros.h>
#include <sysvar.h>
#include <flib.h>
/***************************************************************************************************************************/
#define  CAPT_EXP            200 //曝光时间
#define  CAPT_GAIN_INIT      4600//图像增益

//#define  CAPT_EXP            600 //曝光时间
//#define  CAPT_GAIN_INIT      0xa0//图像增益

#define  CAPT_GAIN_STEP      5
#define  BRIGHT_SET          50
#define  BRIGHT_GAP          3

#define  IniNrOfScreens      4  
#define  ReleaseAllocation  -1
#define  NextScreen          0

#define  GAP_DIS             2

#define  _USE_SMALL_WINDOW_ 

#define RECEIVE_BYTES      3  
#define SERVER_PORT     2002  /* waiting at port 2002 for any connection */
#define MEMORY        500000  /* memory to store one image */

#define  BigWinColumns       400
#define  BigWinRows          300

#define  SmallWinColumns       30	
#define  SmallWinRows          30

#define  MiddleWinColumns      80	
#define  MiddleWinRows         80

#define  dx_win_max     50


#define DILATE_1        5
#define DILATE_2        6
#define DILATE_3        3

#define RLC_MAXLNG      0x50000

#define dimg_threshold  35
#define DEPT_TH         30
#define DCUR_TH         35

#define NOPage 			4
#define NORLC 			4

/***************************************************************************************************************************/

//全局静态变量

static int *Screen,*ScrAdr;
static int Cur_index,BackGrand_index;


static int CacheSrcPage[NOPage],CachePage[NOPage];
static int TempPage;     //存放差帧图像
static int Dcur_bin_Page;//存放差帧二值图像
static int EptBckPage;   //存放空背景图像
static int MaskPage;     //存放空背景差帧图像

static U16 *rlc_cache[NORLC];
static U16 *rlc_source,*rlc_stage1,*rlc_stage2,*rlc_temp;

int x_lable_his[2],y_lable_his[2];
static unsigned char his_far_pos = 0;
static unsigned char his_near_pos = 1;

static int img_gain = CAPT_GAIN_INIT; 

  


//void calculation   (image* area);
//函数声明
int  ScreenBuffer  (int mode,image* area);

void Brightness_Adjust(void);
void mem_init(void);

void TCP_trans(void);
void timedely_ms(int delay);//delay单位为ms，应确保<1000
void mark(int a,int b,char* out);

int Find_brightest_point(image find_win,int* px_lable,int* y_lable);

int Find_pp_pos(image cur_img,image bkg_img,image ept_img,int* px_lable,int* py_lable,int threshold);
int Find_pp_pos_close(image* pcur_smallwin_img,image* pdcur_smallwin_img,image* pdept_smallwin_img,int* px_lable,int* py_lable,int dept_threshold,int dcur_threshold);

int my_dilate(U16* rlc_source,U16* rlc_out,int size);
int my_erode(U16* rlc_source,U16* rlc_out,int size);

void GetCenter(float *pt, U32 count, int *cen);

void upload_lable_his(int x_lable,int y_lable);
int Brightness(image area);

int GSP(image win_img,int* pxlable,int* ylable);
/***************************************************************************************************************************/

void main(void)
{
  int   TimeInit=0,Clock_Time,Clock_Time_last,i;
  int   TimeS = 0;
  char  key=0; 
  image area;
  image cur_img,bkg_img,ept_img; 
  image cur_middlewin_img,bkg_middlewin_img,ert_smallwin_img;
  
  int x_lable = 0,y_lable = 0;
  //int x_lable_ = 0,y_lable = 0;
  
  int X0,Y0;
  int x_ofs,y_ofs;
  
  int wind_flag = 0;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  unsigned char *linebuffer;
  int sendbuf[5];
  sockaddr_in    laddr, raddr;
  unsigned       sock, listensock;
  unsigned       error;
  uint_16        rlen;
  int result;
  
  int ms1,kms;
  
  char  receivedata[RECEIVE_BYTES]; 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  init_licence("T2237AE5748"); 
  // ====================================================================================
  // ====================================================================================
  mem_init();
  // -----------------------------------------
  //开始程序前，拍下空的背景图片,并调节亮度
  Brightness_Adjust();
  //====================================================================================
  OvlClearAll;
  
  print("Press any key to start\n"); 
  print("Press 'q' to quit\n"); 
  print("Press 'c' to clear screen\n");  
  getchar();
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   /* server services port */
   laddr.sin_family      = AF_INET;
   laddr.sin_port        = SERVER_PORT;   /* server listen at port SERVER_PORT (2002) */
   laddr.sin_addr.s_addr = INADDR_ANY;    /* every IP addr can connect to the camera */


   /* use TCP/IP protocoll and search for a free port */
   sock = socket_stream();

   if (sock == VCRT_HANDLE_ERROR) 
     {
     print("\nCreate stream socket failed");
     return;
     } 

   /* connection between free port and selected SERVER_PORT. Now SERVER_PORT is free again for new connections. */
   error = bind(sock, &laddr, sizeof(laddr));

   if (error != VCRT_OK) 
     {
     print("\nStream bind failed - 0x%x", error);
     return;
     }

   /* activate sock for TCP/IP connections */
   error = listen(sock, 0);

   if (error != VCRT_OK) 
     {
     print("\nListen failed - 0x%x", error);
     return;
     }

   listensock = sock;

   print("\n\nFast Ethernet port %d\n",laddr.sin_port);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  while(1)
  {
  	
	    
  	  /* Are there connections at any port? The number 0 means: Wait until connection */
      sock = VCRT_selectall(0);

      /* Is there one who wants to connect to port SERVER_PORT */
      if (sock == listensock)
      {
        /* Connection requested; accept it */
        rlen = sizeof(raddr);

        /* accept connection */
        print("\nwait for accept");
        sock = accept(listensock, &raddr, &rlen);
        if (sock == VCRT_HANDLE_ERROR)
        {
          print("\nAccept failed, error 0x%x",VCRT_geterror(listensock));
          continue;
        }
        else
        {
          print("\naccept ready");
        }

        print("\nConnection from %d.%d.%d.%d, port %d",
           (raddr.sin_addr.s_addr >> 24) & 0xFF,
           (raddr.sin_addr.s_addr >> 16) & 0xFF,
           (raddr.sin_addr.s_addr >>  8) & 0xFF,
            raddr.sin_addr.s_addr        & 0xFF,
            raddr.sin_port);
            
      do
	  {
	    if (kbhit())
	    {
	       key = rs232rcv();
	       if(key == 'c')OvlClearAll;
	    }
		
		sendbuf[0] = 0;
		sendbuf[1] = 0;
		sendbuf[2] = 0;
		sendbuf[3] = 0;
		sendbuf[4] = 0;
		
		recv(sock, (char *)receivedata, RECEIVE_BYTES, 0); 
		if(receivedata[0]=='T'&&receivedata[1]=='C')
		{
			TimeS = 0;
			TimeInit = getvar(MSEC);
			Clock_Time = TimeInit;
			sendbuf[0] = 2;
			sendbuf[3] = Clock_Time;
			linebuffer = (unsigned char*)sendbuf;
			result=send(sock, (char *)linebuffer, 20, 0);
	        if(result==VCRT_ERROR) 
	        {
	          print("\nSend TIMECONFIG_ERR",VCRT_geterror(sock));
	        }
	        continue;
		}
	    /* measure time */	
	    /* take next Screen (mode=0) */
	    ScreenBuffer(NextScreen,&area);
	    
	//	ms1=Clock_Time;
	    Clock_Time = getvar(MSEC) - TimeInit;
	    if(Clock_Time<0)Clock_Time += 1000;
	    if(Clock_Time_last > Clock_Time)TimeS++;
	    if(TimeS==3600)TimeS=0; //每3600s计时器归零一次
	    Clock_Time_last = Clock_Time;
	/*    
	    if(Clock_Time<ms1)
		{
			kms=ms1/1000;
			if((Clock_Time+kms*1000)>ms1)
				Clock_Time+=kms*1000;
			else 
				Clock_Time+=(kms+1)*1000;
		}  
		
	*/	
	 //   print("Clock_Time = %d\n",Clock_Time);      
		//wind_flag = 0;
	    ImageAssign(&cur_img, (long)Screen[Cur_index]		, BigWinColumns, BigWinRows, ScrGetPitch);
		ImageAssign(&bkg_img, (long)Screen[BackGrand_index]	, BigWinColumns, BigWinRows, ScrGetPitch);
		ImageAssign(&ept_img, (long)EptBckPage          	, BigWinColumns, BigWinRows, ScrGetPitch);	
		
	#ifdef _USE_SMALL_WINDOW_
	    if(wind_flag == 1 )//上次找到了乒乓球
	    {
	    	//预测新的窗口位置
	    	x_ofs = x_lable_his[his_near_pos] - x_lable_his[his_far_pos];
	    	if(x_ofs > dx_win_max)x_ofs = dx_win_max; 
	    	else if( x_ofs < -dx_win_max)x_ofs = - dx_win_max;
	    	y_ofs = y_lable_his[his_near_pos] - y_lable_his[his_far_pos];
	    	if(y_ofs > dx_win_max)y_ofs = dx_win_max; 
	    	else if( y_ofs < -dx_win_max)y_ofs = - dx_win_max;
	    	
	    	X0 = x_lable + x_ofs - MiddleWinColumns/2;
			Y0 = y_lable + y_ofs - MiddleWinRows/2;
			if(X0<0)X0=0;
			else if(X0 + MiddleWinColumns >= BigWinColumns)X0=BigWinColumns-1-MiddleWinColumns;
			if(Y0<0)Y0=0;
			else if(Y0 + MiddleWinRows >= BigWinRows)Y0 = BigWinRows-1-MiddleWinRows;
	    	
	    	//=============================================================== 		
			ImageAssign(&cur_middlewin_img, (cur_img.st + X0 + Y0 * ScrGetPitch), MiddleWinColumns, MiddleWinRows, ScrGetPitch);
			ImageAssign(&bkg_middlewin_img, (bkg_img.st + X0 + Y0 * ScrGetPitch), MiddleWinColumns, MiddleWinRows, ScrGetPitch);				
			ImageAssign(&ert_smallwin_img , (ept_img.st + X0 + Y0 * ScrGetPitch), MiddleWinColumns, MiddleWinRows, ScrGetPitch); 
			if( 1 == Find_pp_pos(cur_middlewin_img,bkg_middlewin_img,ert_smallwin_img,&x_lable,&y_lable,DCUR_TH) )
			{	
					wind_flag = 1;
					x_lable += X0;
					y_lable += Y0;
					upload_lable_his(x_lable,y_lable);
					mark(x_lable,y_lable,"*");
					
					sendbuf[0] = 1;
					sendbuf[1] = x_lable*10;
					sendbuf[2] = y_lable*10;
					sendbuf[3] = Clock_Time;
					sendbuf[4] = TimeS;	 	 
			}
			else wind_flag = 0;
		/*	else 
			{
				x_lable = x_lable_his[his_near_pos];
				y_lable = y_lable_his[his_near_pos];
				wind_flag = GSP(cur_img,&x_lable,&y_lable);
				if(wind_flag)
				{
					mark(x_lable,y_lable,"*");
					upload_lable_his(x_lable,y_lable);
				}
				
				sendbuf[0] = 1;
				sendbuf[1] = x_lable;
				sendbuf[2] = y_lable;
				sendbuf[3] = 2;
				sendbuf[4] = 3;	 				
		//		else 
		//		mark(x_lable,y_lable,"O");
			}*/
				/* calculation time */
		}
		if( wind_flag == 0 )//上次没找到乒乓球，或者这次在小窗口和中等窗口都没找到乒乓球,则在大窗口找
	#endif//_USE_SMALL_WINDOW_
		{
			//-------------------------------------------------------------------
			wind_flag = Find_pp_pos(cur_img,bkg_img,ept_img,&x_lable,&y_lable,dimg_threshold);
	
			//这样赋值以确保x_ofs和y_ofs为0
			x_lable_his[his_near_pos] =  x_lable;
			y_lable_his[his_near_pos] =  y_lable;
			x_lable_his[his_far_pos ] =  x_lable;
			y_lable_his[his_far_pos ] =  y_lable;
		//	mark(x_lable,y_lable,"s");
		}
		
		sendbuf[3] = Clock_Time;
		linebuffer = (unsigned char*)sendbuf;
		result=send(sock, (char *)linebuffer, 20, 0);
        if(result==VCRT_ERROR) 
        {
          print("\nSend VCRT_ERROR=%lx",VCRT_geterror(sock));
        }
		              
	  }while(key != 'q');
	   shutdown(sock, FLAG_CLOSE_TX);
	   print("\nshutdown\n");
  }
  print("\nidle\n");
  shutdown(listensock, FLAG_CLOSE_TX);
  break;
}
  
      
  print("Programm Exit.\n");
	  
  vmode(vmLive);
  

  //TCP_trans();
  /* release screen allocation (mode<0) */
  ScreenBuffer(ReleaseAllocation,&area);
  for(i=0;i<NOPage;i++)DRAMPgFree(CacheSrcPage[i]);
  for(i=0;i<NORLC ;i++)vcfree(rlc_cache[i]);
  }

/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/

int ScreenBuffer (int mode,image* area)
  {
  int i;
  static int NrOfScreens=0, *TrackNr, disp, calc, capt;
  image Iniarea;

  /* only possible in vmOvlStill mode */
  vmode(vmOvlStill);

  /* screen allocation mode */
  if (mode>0)
    {
    if (NrOfScreens != 0) return(-5);
    if (mode<3)  return(-4);
    if (mode>20) return(-4);
    
    NrOfScreens=mode;

    ScrAdr=(int *)sysmalloc(NrOfScreens,2); 
    if((int)ScrAdr==0) 
      {
      return(-1);
      }

    Screen=(int *)sysmalloc(NrOfScreens,2); 
    if((int)Screen==0) 
      {
      sysfree((void *)ScrAdr);
      return(-1);
      }

    TrackNr=(int *)sysmalloc(NrOfScreens,2); 
    if((int)TrackNr==0) 
      {
      sysfree((void *)Screen);
      sysfree((void *)ScrAdr);
      return(-1);
      }

    Screen[0]=ScrGetCaptPage;
    for (i=1; i<NrOfScreens; i++)  
      {
      ScrAdr[i]=(int)DRAMDisplayMalloc(); 
      if (ScrAdr[i]==0)
        {
        for (i-=1; i>0; i--)  
          {
          DRAMPgFree(ScrAdr[i]);
          }
        sysfree((void *)TrackNr);
        sysfree((void *)Screen);
        sysfree((void *)ScrAdr);
        return(-1);
        }

      /* Image display addresses have to be in alignments of 1024 */ 
      Screen[i]=(ScrAdr[i]&0xFFFFFC00) + 1024;

     
      ImageAssign(&Iniarea, (long)Screen[i], DispGetColumns, DispGetRows, ScrGetPitch);  
      set(&Iniarea,0);
      }

    for (i=0; i<NrOfScreens-2; i++)  
      {
      TrackNr[i]=capture_request(CAPT_EXP,img_gain,(int *)Screen[i],0);
      if(TrackNr[i]==0) return(-2);
      }
    calc=0;
    disp=NrOfScreens-1;
    capt=NrOfScreens-2;
    }
    
  /* change screen for display, calculation and acquisition */
  if (mode==0)
    {
    if (NrOfScreens==0) return(-3);

    /* waiting for complete image */
    while(TrackNr[calc] > getvar(IMGREADY));

    TrackNr[capt]=capture_request(CAPT_EXP,img_gain,(int *)Screen[capt],0);
    if(TrackNr[capt]==0) return(-2);

    ScrSetDispPage(Screen[disp]);
    ScrSetLogPage (Screen[calc]);
    ImageAssign(area, (long)Screen[calc], DispGetColumns, DispGetRows, ScrGetPitch);
    
    //assert(GAP_DIS < NrOfScreens-1 );
    Cur_index = capt-1;
    if(Cur_index<0)Cur_index = NrOfScreens-1;
    BackGrand_index = Cur_index - GAP_DIS;
    if(BackGrand_index<0)BackGrand_index = NrOfScreens-(GAP_DIS - Cur_index);
    
    capt=disp;
    disp=calc;
    calc++;
    if (calc==NrOfScreens) calc=0;
    
    
    }


  /* release screen allocation mode */
  if (mode<0)
    {
    if (NrOfScreens==0) return(-3);
 
    ScrSetPhysPage(Screen[0]);

    for (i=NrOfScreens-1; i>0; i--)  
      {
      DRAMPgFree(ScrAdr[i]);
      }

    NrOfScreens=0;

    sysfree((void *)TrackNr);
    sysfree((void *)Screen);
    sysfree((void *)ScrAdr);
    
    ScrSetLogPage(Screen[capt]);
    vmode(vmLive);
    }
    
    
  return(0);
  }
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
void TCP_trans(void)
{
   unsigned char *linebuffer, *plinebuf, *pixel;
   char  receivedata[RECEIVE_BYTES]; 
   int  i, j, x, y, dx, dy, incrx, incry, result, ms, count, ScreenIndex=0;
   long sec;
   sockaddr_in    laddr, raddr;
   unsigned       sock, listensock;
   unsigned       error;
   uint_16        rlen;


   /* transfer the image from the physical page */
  // ScrSetLogPage((int)ScrGetPhysPage);


   /* temporarily work around */
 //  tpict();
  // setvar(DISP_ACTIVE,0);


   /* allocate memory for image raw data */
   linebuffer=(unsigned char *)sysmalloc(MEMORY,3);
   if((int)linebuffer==0) print("not enough memory\n");


   /* server services port */
   laddr.sin_family      = AF_INET;
   laddr.sin_port        = SERVER_PORT;   /* server listen at port SERVER_PORT (2002) */
   laddr.sin_addr.s_addr = INADDR_ANY;    /* every IP addr can connect to the camera */


   /* use TCP/IP protocoll and search for a free port */
   sock = socket_stream();

   if (sock == VCRT_HANDLE_ERROR) 
     {
     print("\nCreate stream socket failed");
     return;
     } 

   /* connection between free port and selected SERVER_PORT. Now SERVER_PORT is free again for new connections. */
   error = bind(sock, &laddr, sizeof(laddr));

   if (error != VCRT_OK) 
     {
     print("\nStream bind failed - 0x%x", error);
     return;
     }

   /* activate sock for TCP/IP connections */
   error = listen(sock, 0);

   if (error != VCRT_OK) 
     {
     print("\nListen failed - 0x%x", error);
     return;
     }

   listensock = sock;

   print("\n\nFast Ethernet port %d\n",laddr.sin_port);

   for (;;) {

      /* Are there connections at any port? The number 0 means: Wait until connection */
      sock = VCRT_selectall(0);

      /* Is there one who wants to connect to port SERVER_PORT */
      if (sock == listensock)
      {
        /* Connection requested; accept it */
        rlen = sizeof(raddr);

        /* accept connection */
        print("\nwait for accept");
        sock = accept(listensock, &raddr, &rlen);
        if (sock == VCRT_HANDLE_ERROR)
        {
          print("\nAccept failed, error 0x%x",VCRT_geterror(listensock));
          continue;
        }
        else
        {
          print("\naccept ready");
        }

        print("\nConnection from %d.%d.%d.%d, port %d",
           (raddr.sin_addr.s_addr >> 24) & 0xFF,
           (raddr.sin_addr.s_addr >> 16) & 0xFF,
           (raddr.sin_addr.s_addr >>  8) & 0xFF,
            raddr.sin_addr.s_addr        & 0xFF,
            raddr.sin_port);


        do
        {
          print("\n\nWaiting for image size");
		
		 x=0;
		 y=0;
         dx=600;
         dy=480;
         incrx = 1;
         incry = 1;
         
         count = recv(sock, (char *)receivedata, RECEIVE_BYTES, 0); 
          ScreenIndex=0;

          if(count == VCRT_ERROR)
            print("\nVCRT_ERROR 0x%x",VCRT_geterror(sock));
          else
            if(count < RECEIVE_BYTES)
               print("\nonly %d bytes read",count);
          else /* no errors*/
          { 
          	ScreenIndex=receivedata[0];
          }

        /* dx=0 : quit program */
        if (ScreenIndex != 'e')
        {
         // tpict();
         //ImageAssign(&trans_img, (long)Screen[ScreenIndex], DispGetColumns, DispGetRows, ScrGetPitch);
		  print("\nTrains NO.%d image",(int)ScreenIndex);
		  ScrSetLogPage(TempPage);
			
          plinebuf=linebuffer; /* 4 Bytes for length and 24 Bytes for image data */

          count=0;

          for(j=0;j<dy;j+=incry)
          {
            pixel=(unsigned char *)ScrByteAddr(x,y+j);

            for(i=0;i<dx;i+=incrx) 
            {
              *plinebuf++ = *pixel;
               pixel+=incrx;
               count++;
            }
          }

          sec = getvar(SEC);
          ms  = getvar(MSEC);
           
          result=send(sock, (char *)linebuffer, count, 0);
          if(result==VCRT_ERROR) 
          {
            print("\nSend VCRT_ERROR=%lx",VCRT_geterror(sock));
          }
          else
          {
            if(result!=count)    
            print("\nSend error wrong length=%d result=%d",count,result);
          }

          sec = getvar(SEC)-sec;
          ms  = getvar(MSEC)-ms;
            
          ms = 1000*(int)sec + ms;
          print(" \n trans t=%dms",ms);
        }
        }
        while(ScreenIndex!='e');

        shutdown(sock, FLAG_CLOSE_TX);
        };

     break;

   } /* Endfor */
   shutdown(listensock, FLAG_CLOSE_TX);

   sysfree(linebuffer);


   /* activate image display */
 //  setvar(DISP_ACTIVE,1);

vmode(vmLive);

} 

void timedely_ms(int delay)//delay单位为ms，应确保<1000
{
	int TimeAllMs,Timeinit;
	Timeinit = getvar(MSEC);
	do
	{
		TimeAllMs = getvar(MSEC)-Timeinit; 
    	if (TimeAllMs<0) TimeAllMs+=1000;
	}while(delay>=TimeAllMs);
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
void mark(int a,int b,char *out)
{
	image OvlArea;
	OvlSetLogPage((int)OvlGetPhysPage);
	ImageAssign(&OvlArea,OvlBitAddr(a,b),5,5,OvlGetPitch);
	set_overlay_bit(2,255,0,0);
	set(&OvlArea,4);	
	chprint1(out,&OvlArea,1,1,4);
	set_ovlmask(255);
}


int Find_brightest_point(image find_win,int* px_lable,int* py_lable)
{
	unsigned char * pixel;
	int i,j,max=0;
	int dx,dy,pitch;
	dx = find_win.dx;
	dy = find_win.dy;
	pitch = find_win.pitch;
	for(j=0;j<dy;j++)
	{
		pixel=find_win.st + j*pitch;
		for(i=0;i<dx;i++)
		{
			if(max<(*pixel))
			{
				max = *pixel;
				*px_lable = i;
				*py_lable = j;
			}
			pixel++;
		}
	}
	if(max < 10 )return 0;
	else return max;
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
int Find_pp_pos(image cur_img,image bkg_img,image ept_img,int* px_lable,int* py_lable,int threshold)
{
	//==========================
	U16* slc;
	unsigned char * pixel,*pixel_mask;
	int i,j,X0,Y0 ;
	
	int possible_pos[10][2];
    int possible_pos_index = 0;
      
    image dcur_img,dcur_bin_img;
    image cur_testwin_img,dept_testwin_img,ept_testwin_img,dcur_testwin_img,blk_mask;
    
	
    

    //------------------  
	//==========================
	//------------------------------
	possible_pos_index = 0;
	for(i=0;i<10;i++)
	{
		possible_pos[i][0]=0;
		possible_pos[i][1]=0;
	}
	//------------------------------
	//====================================================
	ImageAssign(&dcur_img, (long)TempPage, cur_img.dx,cur_img.dy,cur_img.pitch);
	
	sub2(&cur_img,&bkg_img,&dcur_img);
				
	slc=rlcmk(&dcur_img, threshold, rlc_source, RLC_MAXLNG);
    if (slc == NULL ) {pstr("RLC overrun\n"); vcfree((int*)rlc_source); return -1;}
    
   // my_erode(rlc_source,rlc_stage2,1);
   // my_dilate(rlc_stage2,rlc_source,1);
    
/*	my_dilate(rlc_source,rlc_stage1,DILATE_1);
	rlc_inv(rlc_stage1);
	my_dilate(rlc_stage1,rlc_stage2,DILATE_2);
	rlcand(rlc_source,rlc_stage2,rlc_stage1);
	my_dilate(rlc_stage1,rlc_stage2,DILATE_3);
*/	my_dilate(rlc_source,rlc_stage2,DILATE_3);
	
	ImageAssign(&dcur_bin_img, (long)Dcur_bin_Page, cur_img.dx,cur_img.dy,cur_img.pitch);
	rlcout(&dcur_bin_img,rlc_stage2,0,255);
	//====================================================
	for(j=0;j<dcur_bin_img.dy ;j+=1)
    {
            pixel=dcur_bin_img.st + j*dcur_bin_img.pitch;

            for(i=0;i<dcur_bin_img.dx;i+=1) 
            {
           
               if(*pixel == 255 && possible_pos_index<10)
			   {
				//	possible_pos[possible_pos_index][0] = i;
				//	possible_pos[possible_pos_index][1] = j;
					
					//在可疑位置处开辟小窗口
					X0 = i  - SmallWinColumns/3;
					Y0 = j  - SmallWinRows/3;
					if(X0<0)X0=0;
					else if( (X0 + SmallWinColumns) >= BigWinColumns-1 )
					X0 = BigWinColumns - 1 - SmallWinColumns;
					if(Y0<0)Y0=0;
					else if( (Y0 + SmallWinRows   ) >= BigWinRows-1 )
					Y0 = BigWinRows - 1 - SmallWinRows;
		 			
		 			ImageAssign(&cur_testwin_img, (long)cur_img.st    +X0+Y0*cur_img.pitch     , SmallWinColumns     ,SmallWinRows     , cur_img.pitch     );
					ImageAssign(&dcur_testwin_img,(long)dcur_img.st   +X0+Y0*dcur_img.pitch    , SmallWinColumns    ,SmallWinRows    , dcur_img.pitch    );
					ImageAssign(&ept_testwin_img, (long)ept_img.st    +X0+Y0*ept_img.pitch     , SmallWinColumns     ,SmallWinRows     , ept_img.pitch     );
					ImageAssign(&dept_testwin_img,(long)MaskPage,                                SmallWinColumns    ,SmallWinRows     , ept_img.pitch     );					
					
					//做现有图像与空背景之差
					sub2(&cur_testwin_img,&ept_testwin_img,&dept_testwin_img);
					if(Find_pp_pos_close(&cur_testwin_img,&dcur_testwin_img,&dept_testwin_img,&possible_pos[possible_pos_index][0],&possible_pos[possible_pos_index][1],DEPT_TH,DCUR_TH) )
					{
						*px_lable =  possible_pos[possible_pos_index][0];
	  					*py_lable =  possible_pos[possible_pos_index][1];
	  					*px_lable += X0;
	  					*py_lable += Y0;
						return 1;
					}
					else//若找到点没有通过测试，则把这一片涂黑
					{
						ImageAssign(&blk_mask, (long)dcur_bin_img.st+X0+Y0*dcur_bin_img.pitch,SmallWinColumns,SmallWinRows,dcur_bin_img.pitch     );
						for(j=0;j<blk_mask.dy ;j+=1)
					    {
					        pixel_mask=blk_mask.st + j*blk_mask.pitch;
						    for(i=0;i<blk_mask.dx;i+=1) 					           
					 		{
					 			(*pixel_mask)=0;
					 			pixel_mask++;
					        }
					     }
					}
					
					possible_pos_index++;	
					
													
				}

				pixel+=1;
            }
      }

      
	  *px_lable = 0;
	  *py_lable = 0;
	  return 0;	  
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/



/***************************************************************************************************************************/
/***************************************************************************************************************************/
//在差幁图像与空背景差图像同时检测到球的位置找到最亮点，然后在原图中生长
int Find_pp_pos_close(image* pcur_smallwin_img,image* pdcur_smallwin_img,image* pdept_smallwin_img,int* px_lable,int* py_lable,int dept_threshold,int dcur_threshold)
{
	unsigned char * pixel_dept,*pixel_dcur,*pixel_cur;
	int i,j,max=0;
	int dx,dy,pitch;

	
	    	    
	dx = pcur_smallwin_img->dx;
	dy = pcur_smallwin_img->dy;
	pitch = pcur_smallwin_img->pitch;
	for(j=0;j<dy;j++)
	{
		pixel_dcur = pdcur_smallwin_img->st + j*pitch;
		pixel_dept = pdept_smallwin_img->st + j*pitch;
		pixel_cur  = pcur_smallwin_img->st  + j*pitch;
		for(i=0;i<dx;i++)
		{
			if( dcur_threshold < (*pixel_dcur) && max < (*pixel_dept)  )
			{
				max = *pixel_dept;
				*px_lable = i;
				*py_lable = j;
			}
			pixel_dept++;
			pixel_dcur++;
			pixel_cur++;
		}
	}

    	
	if(max < dept_threshold )return 0;
	else 
	{
		return GSP(*pdept_smallwin_img,px_lable,py_lable);
	}
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
int GSP(image win_img,int* pxlable,int* pylable)
{
	unsigned char * pixel_init,*pixel;
	int i,j,max=0;
	int size_pp_max = 10;

	int x_init,y_init;
	int up[3];
	int down[3];
	int left[3];
	int right[3];
	
	int wide,high;

	    
	x_init = *pxlable;
	y_init = *pylable;
	
	pixel_init = win_img.st + x_init + (y_init)*win_img.pitch;
	max = *(pixel_init);
	

	
	for(j=0;j<3;j++)
	{
		pixel_init = win_img.st + x_init+(j-1) + (y_init)*win_img.pitch;
		
		for(i=0;i<size_pp_max;i++)
		{
			pixel = pixel_init + i;
		//	if( *pixel < 0.5*(*pixel_init) )break;
		if( *pixel < max/2 )break;
		}
		right[j] = i;
		for(i=0;i<size_pp_max;i++)
		{
			pixel = pixel_init - i;
			if( *pixel < max/2  )break;
		}
		left[j] = -i; 
		
	}
	for(j=0;j<3;j++)
	{
		pixel_init = win_img.st + x_init + (y_init + j-1 )*win_img.pitch;		
		for(i=0;i<size_pp_max;i++)
		{
			pixel = pixel_init + i*win_img.pitch;
			if( *pixel < max/2  )break;
		}
		down[j] = i;
		for(i=0;i<size_pp_max;i++)
		{
			pixel = pixel_init - i*win_img.pitch;
			if( *pixel < max/2  )break;
		}
		up[j] = -i;
	}
    	
    	
	high = -(up[0]+   up[1]+  up[2]   - down[0] -down[1] -down[2]);
	wide = -(left[0]+ left[1]+left[2] - right[0]-right[1]-right[2]);
	if( 	high >(size_pp_max * 3 * 1.65) 
		||  high <(size_pp_max * 3 * 0.35)
		||  wide >(size_pp_max * 3 * 1.65) 
		||  wide <(size_pp_max * 3 * 0.35)
		||  high - wide >12
		||  wide - high >12  )
	{return 0;}
	
//	print("\n w=	%d,h = %d\n",wide,high);
	 
	*pxlable = (left[0]+ left[1]+left[2]+right[0]+right[1]+right[2])/6 + x_init;
	*pylable = (up[0]  + up[1]  +up[2]  +down[0] +down[1]  +down[2])/6 + y_init;
	return 1;
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
int my_dilate(U16* rlc_source,U16* rlc_out,int size)
{
	U16 * rlc_1,*rlc_2,*rlc_3;
	int i;
	if( size <= 1 )
	{
		dilate(rlc_source,rlc_out);
		return 1;
	}	
	else
	{
		rlc_2 = rlc_source;
		rlc_3 = rlc_temp;
		for(i=0;i<size-2;i++)
		{
			rlc_1 = rlc_2;
			rlc_2 = rlc_3;
			rlc_3 = dilate(rlc_1,rlc_2);
		}
		dilate(rlc_2,rlc_out);
		return 1;
	}
	
}

int my_erode(U16* rlc_source,U16* rlc_out,int size)
{
	U16 * rlc_1,*rlc_2,*rlc_3;
	int i;
	if( size <= 1 )
	{
		erode(rlc_source,rlc_out);
		return 1;
	}	
	else
	{
		rlc_2 = rlc_source;
		rlc_3 = rlc_temp;
		for(i=0;i<size-2;i++)
		{
			rlc_1 = rlc_2;
			rlc_2 = rlc_3;
			rlc_3 = erode(rlc_1,rlc_2);
		}
		erode(rlc_2,rlc_out);
		return 1;
	}
	
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
void upload_lable_his(int x_lable,int y_lable)
{
	his_far_pos = ((his_far_pos ==1)?0:1);
	his_near_pos =((his_near_pos==1)?0:1);
	x_lable_his[his_near_pos] = x_lable;
	y_lable_his[his_near_pos] = y_lable;
}
/***************************************************************************************************************************/
/***************************************************************************************************************************/







/***************************************************************************************************************************/
/***************************************************************************************************************************/
int Brightness(image area)
{
	int i,j,sum=0;
	unsigned char * pixel;
	for(j=0;j<area.dy ;j+=1)
    {
            pixel=area.st + j*area.pitch;

            for(i=0;i<area.dx;i+=1) 
            {
				sum += (int)(*pixel);
				pixel+=1;
            }
     }
     sum/= (area.dx * area.dy);
     return sum;
}

void Brightness_Adjust(void)
{
  image area,ept_bck_img;
  unsigned char *pixel,*pixel_ept;
  int BrightnessVal;
  int m,n,i;
  
  ImageAssign(&ept_bck_img, (long)EptBckPage, BigWinColumns, BigWinRows, ScrGetPitch);  
  print("\n拍摄空背景:清空球台,按任意键开始拍摄\n"); 
  getchar();
  
  while(1)
  {
  	for(m=0;m<ept_bck_img.dy;m++)
	{
		pixel_ept = ept_bck_img.st + m*ept_bck_img.pitch;
		for(n=0;n<ept_bck_img.dx;n++)
		{
			(*pixel_ept) =0;
			pixel_ept++;
		}
	} 	
  	for(i=0;i<5;i++)
  	{
  		ScreenBuffer(NextScreen,&area);
  		for(m=0;m<area.dy;m++)
		{
			pixel=area.st + m*area.pitch;
			pixel_ept = ept_bck_img.st + m*ept_bck_img.pitch;
			for(n=0;n<area.dx;n++)
			{
				(*pixel_ept) += (*pixel)/5;
				pixel++;
				pixel_ept++;
			}
		}		
  	}
  	
	BrightnessVal = Brightness(ept_bck_img);

    	
  	if( BrightnessVal - BRIGHT_SET > BRIGHT_GAP )
  	{
  		img_gain -= CAPT_GAIN_STEP;
  		print("%d - \n",BrightnessVal);
  		continue;
  	}
  	else if(BRIGHT_SET - BrightnessVal > BRIGHT_GAP)
  	{
  		img_gain += CAPT_GAIN_STEP;
  		print("%d + \n",BrightnessVal);
  		continue;
  	}
  	break;
  }
  print("\n亮度调整结束,亮度为 %d \n",BrightnessVal);
}

void mem_init(void)
{
  int i,result;
  image area;
  	
  for(i=0;i<NORLC;i++)
  {
  	rlc_cache[i] =(U16*)vcmalloc(RLC_MAXLNG);
  	if (rlc_cache[i] ==NULL ) {pstr("RLC Memory overrun\n"); return;}
  }
  
  rlc_source = rlc_cache[0];
  rlc_stage1 = rlc_cache[1];
  rlc_stage2 = rlc_cache[2];
  rlc_temp   = rlc_cache[3];
  //----------------------------------------------- 
  for(i=0;i<NOPage;i++)
  {
  	CacheSrcPage[i] = (int)DRAMDisplayMalloc();   //差帧图像
  	CachePage[i]    = (CacheSrcPage[i]&0xFFFFFC00) + 1024;
  }
    
  TempPage  = CachePage[0];
  Dcur_bin_Page = CachePage[1];
  EptBckPage= CachePage[2];
  MaskPage  = CachePage[3];
  
  //-----------------------------------------------
  //初始化，为高速拍摄分配内存
  result = ScreenBuffer(IniNrOfScreens,&area);
  if (result < 0) {print("Screen Allocation Error %d\n",result); return;}
}
