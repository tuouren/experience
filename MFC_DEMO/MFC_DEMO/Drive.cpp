/***********************************************************************/

/*  Project         -     Robot Drive                                           */

/*  File Name       -     drive.cpp                                             */

/*  Purpose         -     implement the driving function of robot platform.     */

/*  Author          -     Li  Yuan                                              */

/*  Date            -     2008/03/13                                             */

/*  Version         -     1.0                                                   */

/*********************************************************************************/

#include "stdafx.h"
#include  "math.h"
#include "Drive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDrive

CDrive::CDrive()
{
}

CDrive::~CDrive()
{
}


BEGIN_MESSAGE_MAP(CDrive, CWnd)
	//{{AFX_MSG_MAP(CDrive)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrive message handlers

/*****************************************************************************/
/*	
void drive_init(CMSComm* m_comm, UINT driveportnum);
void drive_init(CMSComm* m_comm, UINT driveportnum,short acceleration);

void drive_forward_line(CMSComm* m_comm, float AverageV);
void drive_forward_curve1(CMSComm* m_comm, float AverageV,float VAngle);
void drive_forward_curve2(CMSComm* m_comm, float V,float Omega);

void drive_back_line(CMSComm* m_comm, float AverageV);
void drive_back_curve1(CMSComm* m_comm, float AverageV,float VAngle);
void drive_back_curve2(CMSComm* m_comm, float V,float Omega);

void drive_leftright_velocity(CMSComm* m_comm, float LeftV,float RightV);

void drive_rotate_velocity(CMSComm* m_comm, float AverageV,bool ifright);
void drive_rotate_angle(CMSComm* m_comm, float AverageV,short RotateAngle,bool FinishReturn);

void drive_forward_distance(CMSComm* m_comm, float AverageV,short Distance,bool FinishReturn);
void drive_forward_position1(CMSComm* m_comm, float AverageV,float VAngle,short Distance,bool FinishReturn);
void drive_forward_position2(CMSComm* m_comm, float V,float Omega,short Distance,bool FinishReturn);

void drive_back_distance(CMSComm* m_comm, float AverageV,short Distance,bool FinishReturn);
void drive_back_position1(CMSComm* m_comm, float AverageV,float VAngle,short Distance,bool FinishReturn);
void drive_back_position2(CMSComm* m_comm, float V,float Omega,short Distance,bool FinishReturn);

void drive_move_position(CMSComm* m_comm, float LeftV,float RightV,short Distance,bool FinishReturn);

void drive_status_query(CMSComm* m_comm);
void drive_set_startpoint(CMSComm* m_comm);
void drive_local_distance_query(CMSComm* m_comm);
void drive_global_distance_query(CMSComm* m_comm);				
 *//************************************************************************************************/

void CDrive::drive_init(CMSComm* m_comm, UINT driveportnum)
{
	//打开串口
	if(m_comm->GetPortOpen())	
		m_comm->SetPortOpen(FALSE);
	m_comm->SetCommPort(driveportnum);
	if(!m_comm->GetPortOpen() )  	
		m_comm->SetPortOpen(TRUE);
	else
		AfxMessageBox("cannot open serial port");

	//设置串口
	m_comm->SetSettings("38400,N,8,1");
	m_comm->SetInputMode(1); 
	m_comm->SetRThreshold(8);
	m_comm->SetInputLen(32);
	m_comm->GetInput();
			
	// 设置加速度 ------------------------------
	short A;
	A=3;
	byteBuf[0]=CMD_INIT;	//4+1 in all
	ppos=&A;
	pbyte=(BYTE*)ppos;
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	OutBuf.SetSize(3); 		
	for(i=0;i<3;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 

	//停止命令
	//drive_forward_line(m_comm, 0.0);
}

void CDrive::drive_init(CMSComm* m_comm, UINT driveportnum,short acceleration)
{
	//打开串口
	if(m_comm->GetPortOpen())	
		m_comm->SetPortOpen(FALSE);
	m_comm->SetCommPort(driveportnum);
	if(!m_comm->GetPortOpen() )  	
		m_comm->SetPortOpen(TRUE);
	else
		AfxMessageBox("cannot open serial port");
	
	//设置串口
	m_comm->SetSettings("38400,N,8,1");
	m_comm->SetInputMode(1); 
	m_comm->SetRThreshold(8);
	m_comm->SetInputLen(32);
	m_comm->GetInput();
	
	// 设置加速度 ------------------------------
	short A;
	A=acceleration;
	if(A>5)
	{
		A=5;
	}
	else if(A<1)
	{
		A=1;
	}
	byteBuf[0]=CMD_INIT;	//4+1 in all
	ppos=&A;
	pbyte=(BYTE*)ppos;
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	OutBuf.SetSize(3); 		
	for(i=0;i<3;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	//停止命令
	//drive_forward_line(m_comm, 0.0);

}
void CDrive::drive_forward_line(CMSComm* m_comm, float AverageV)
{
   float V,vleft,vright;//平均速度，左右轮速度

   //输入速度处理,V的范围0<AverageV<MAXVELOCITY
   V=AverageV;
   if(V>MAXVELOCITY)
   {
	   V=MAXVELOCITY;
   }
   else if(V<0)
   {
	   V=0;
   }

   //计算左右轮速度
	V=(float) (60*V/(PI*WHEELD)); //cm/s->r/m
	vleft=V;
	vright=V;
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;

	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;

	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;

}

void CDrive::drive_forward_curve1(CMSComm* m_comm, float AverageV,float VAngle)
{
	float vleft,vright;//左右轮速度
	double w;//角速度
	
	//输入速度处理
	if(AverageV<0)
	{
		vleft=0;
		vright=0;
	}
    else
	{
		vleft=float(AverageV+ROBOTW*tan(VAngle*PI/180)/2);
		vright=float(AverageV-ROBOTW*tan(VAngle*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}

	//左右轮速度   cm/s->r/m
	vleft=(float) (60*vleft/(PI*WHEELD));
	vright=(float) (60*vright/(PI*WHEELD));
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	
	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;	
	
	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;

}
void CDrive::drive_forward_curve2(CMSComm* m_comm, float V,float Omega)
{
	float vleft,vright;//左右轮速度
	double w;//角速度
	
	//输入速度处理
	if(V<0)
	{
		vleft=0;
		vright=0;
	}
	else
	{
		vleft=float(V+ROBOTW*(Omega*PI/180)/2);
		vright=float(V-ROBOTW*(Omega*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}

	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//左右轮速度   cm/s->r/m
	vleft=(float) (60*vleft/(PI*WHEELD));
	vright=(float) (60*vright/(PI*WHEELD));

    
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	
	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;
		
	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;
}

void CDrive::drive_back_line(CMSComm* m_comm, float AverageV)
{
   float V,vleft,vright;//平均速度，左右轮速度
   
   //输入速度处理
   V=AverageV;
   if(V<0)
   {
	   V=0;
   }
   if(V>MAXVELOCITY)
   {
	   V=MAXVELOCITY;
   }
   
   //计算左右轮速度
   V=(float) (60*V/(PI*WHEELD)); //cm/s->r/m
   vleft=-V;
   vright=-V;
   
   
   //设置发送数据
   byteBuf[0]=CMD_VELOCITY;	
   
   pvel=&vleft;
   pbyte=(BYTE*)pvel;
   byteBuf[3]=*pbyte++;		
   byteBuf[4]=*pbyte++;		
   byteBuf[1]=*pbyte++;		
   byteBuf[2]=*pbyte;
   
   pvel=&vright;
   pbyte=(BYTE*)pvel;
   byteBuf[7]=*pbyte++;		
   byteBuf[8]=*pbyte++;		
   byteBuf[5]=*pbyte++;		
   byteBuf[6]=*pbyte;		
   
   //发送
   OutBuf.SetSize(9); 		
   for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
   m_comm->SetOutput(COleVariant(OutBuf)); 
   
   return;

}

void CDrive::drive_back_curve1(CMSComm* m_comm, float AverageV,float VAngle)
{
	float vleft,vright;//左右轮速度
	double w;//角速度
	
	//输入速度处理
	if(AverageV<0)
	{
		vleft=0;
		vright=0;
	}
    else
	{
		vleft=float(AverageV-ROBOTW*tan(VAngle*PI/180)/2);
		vright=float(AverageV+ROBOTW*tan(VAngle*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}

	
	//左右轮速度   cm/s->r/m
	vleft=(float) (-60*vleft/(PI*WHEELD));
	vright=(float) (-60*vright/(PI*WHEELD));
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	
	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;	
	
	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;
}
void CDrive::drive_back_curve2(CMSComm* m_comm, float V,float Omega)
{
	float vleft,vright;//左右轮速度
	double w;//角速度
	
	//输入速度处理
	if(V<0)
	{
		vleft=0;
		vright=0;
	}
	else
	{
		vleft=float(V-ROBOTW*(Omega*PI/180)/2);
		vright=float(V+ROBOTW*(Omega*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//左右轮速度   cm/s->r/m
	vleft=(float) (-60*vleft/(PI*WHEELD));
	vright=(float) (-60*vright/(PI*WHEELD));
	
    
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	
	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;
	
	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;
}

void CDrive::drive_leftright_velocity(CMSComm* m_comm, float LeftV,float RightV)
{
	float vleft,vright;//左右轮速度
	double w;//角速度
	
	//输入速度处理
    vleft=LeftV;
	vright=RightV;
	
	if(vleft>MAXVELOCITY)
	{
		vleft=MAXVELOCITY;
	}
	else if(vleft<-MAXVELOCITY)
	{
		vleft=-MAXVELOCITY;
	}

	if(vright>MAXVELOCITY)
	{
		vright=MAXVELOCITY;
	}
	else if(vright<-MAXVELOCITY)
	{
		vright=-MAXVELOCITY;
	}

	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//计算左右轮速度   cm/s->r/m
	vleft=(float) (60*vleft/(PI*WHEELD));
	vright=(float) (60*vright/(PI*WHEELD));
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY;	
	
	pvel=&vleft;
	pbyte=(BYTE*)pvel;
	byteBuf[3]=*pbyte++;		
	byteBuf[4]=*pbyte++;		
	byteBuf[1]=*pbyte++;		
	byteBuf[2]=*pbyte;
	
	pvel=&vright;
	pbyte=(BYTE*)pvel;
	byteBuf[7]=*pbyte++;		
	byteBuf[8]=*pbyte++;		
	byteBuf[5]=*pbyte++;		
	byteBuf[6]=*pbyte;
	
	//发送
	OutBuf.SetSize(9); 		
	for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	m_comm->SetOutput(COleVariant(OutBuf)); 
	
	return;	
}

void CDrive::drive_rotate_velocity(CMSComm* m_comm, float AverageV,bool ifright)
{
	   float V,vleft,vright;//平均速度，左右轮速度
	   double w;//角速度

	   
	   //输入速度处理
	   V=AverageV;
	   if(V<0)
	   {
		   V=0;
	   }
	   if(V>MAXVELOCITY/2)
	   {
		   V=MAXVELOCITY/2;
	   }
	   
	   //计算左右轮速度
	   V=(float) (60*V/(PI*WHEELD)); //cm/s->r/m
	   if(ifright)
	   {
		   vleft=V;
		   vright=-V;   
	   }
	   else
	   {
		   vleft=-V;
		   vright=V; 
	   }

	   w=(vleft-vright)*180/(ROBOTW*PI);
	   if(w>MAXOMEGA)
	   {
		   vleft=ROTATEVELOCITY;
		   vright=-ROTATEVELOCITY;		
	   }
	   else if(w<-MAXOMEGA)
	   {
		   vleft=-ROTATEVELOCITY;
		   vright=ROTATEVELOCITY;	
	   }
	   
	   //计算左右轮速度   cm/s->r/m
	   vleft=(float) (60*vleft/(PI*WHEELD));
	   vright=(float) (60*vright/(PI*WHEELD));
	   
	   
	   //设置发送数据
	   byteBuf[0]=CMD_VELOCITY;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;		
	   
	   //发送
	   OutBuf.SetSize(9); 		
	   for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}
void CDrive::drive_rotate_angle(CMSComm* m_comm, float AverageV,short RotateAngle,bool FinishReturn)
{
	   float V,vleft,vright;//平均速度，左右轮速度
       short Angle;
	   double w;
	   short returnflag;

	   if(FinishReturn)
	   {
			returnflag=1;
	   }
	   else
	   {
			returnflag=0;
	   }
	   
	   //输入速度处理
	   V=AverageV;
	   if(V<0)
	   {
		   V=0;
	   }
	   if(V>MAXVELOCITY/2)
	   {
		   V=MAXVELOCITY/2;
	   }
	   
	   
	   //计算左右轮速度
	   if(RotateAngle>0)
	   {
		   vleft=V;
		   vright=-V;   
	   }
	   else
	   {
		   vleft=-V;
		   vright=V; 
	   }

	   w=(vleft-vright)*180/(ROBOTW*PI);
	   if(w>MAXOMEGA)
	   {
		   vleft=ROTATEVELOCITY;
		   vright=-ROTATEVELOCITY;		
	   }
	   else if(w<-MAXOMEGA)
	   {
		   vleft=-ROTATEVELOCITY;
		   vright=ROTATEVELOCITY;	
	   }
	   
       if(RotateAngle>0)
	   {
		   V=(float) fabs((60*vleft/(PI*WHEELD)));
		   Angle=-short(fabs((double)RotateAngle));
	   }
	   else
	   {
		   V=-(float) fabs((60*vleft/(PI*WHEELD)));
		   Angle=short(fabs((double)RotateAngle));
	   }
	   
	   //设置发送数据
	   byteBuf[0]=CMD_VELOCITY_ANGLE;	
	   
	   pvel=&V;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   ppos=&Angle;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;

	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[7]=*pbyte++;
	   byteBuf[8]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(9);
	   m_comm->SetRThreshold(9);
	   for(i=0;i<9;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_forward_distance(CMSComm* m_comm, float AverageV,short Distance,bool FinishReturn)
{
	   float V,vleft,vright;//平均速度，左右轮速度
	   short LD,RD;
	   short returnflag;
	   
	   if(FinishReturn)
	   {
		   returnflag=1;
	   }
	   else
	   {
		   returnflag=0;
	   }

	   LD=Distance;
	   RD=Distance;
	   if(LD<0)
	   {
		   LD=0;
	   }
	   if(RD<0)
	   {
		   RD=0;
	   }

	   //输入速度处理,V的范围0<AverageV<MAXVELOCITY
	   V=AverageV;
	   if(V>MAXVELOCITY)
	   {
		   V=MAXVELOCITY;
	   }
	   else if(V<0)
	   {
		   V=0;
	   }
	   
	   //计算左右轮速度
	   V=(float) (60*V/(PI*WHEELD)); //cm/s->r/m
	   vleft=V;
	   vright=V;
	   
	   //设置发送数据
	   byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;

	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;

	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_forward_position1(CMSComm* m_comm, float AverageV,float VAngle,short Distance,bool FinishReturn)
{
	float vleft,vright;//左右轮速度
	short D;
	double w;//角速度
	short LD,RD;
	short returnflag;
	   
   if(FinishReturn)
   {
	   returnflag=1;
   }
   else
   {
	   returnflag=0;
   }
	   
   //输入速度和距离处理
   D=Distance;
   if(D<0)
   {
	   D=0;
   }
 
   //输入速度处理
   if(AverageV<0)
   {
	   vleft=0;
	   vright=0;
   }
   else
   {
	   vleft=float(AverageV+ROBOTW*tan(VAngle*PI/180)/2);
	   vright=float(AverageV-ROBOTW*tan(VAngle*PI/180)/2);
	   if(vleft<-MAXVELOCITY)
	   {
		   vleft=-MAXVELOCITY;
	   }
	   else if(vleft>MAXVELOCITY)
	   {
		   vleft=MAXVELOCITY;
	   }
	   if(vright<-MAXVELOCITY)
	   {
		   vright=-MAXVELOCITY;
	   }
	   else if(vright>MAXVELOCITY)
	   {
		   vright=MAXVELOCITY;
	   }
   }
   
   w=(vleft-vright)*180/(ROBOTW*PI);
   if(w>MAXOMEGA)
   {
	   vleft=ROTATEVELOCITY;
	   vright=-ROTATEVELOCITY;		
   }
   else if(w<-MAXOMEGA)
   {
	   vleft=-ROTATEVELOCITY;
	   vright=ROTATEVELOCITY;	
   }
   
   //左右轮速度   cm/s->r/m
   vleft=(float) (60*vleft/(PI*WHEELD));
   vright=(float) (60*vright/(PI*WHEELD));

   float CV;
   CV=(vleft+vright)/2;
   if(CV==0)
   {
	   LD=0;
	   RD=0;	   
   }
   else
   {
	   LD=short(vleft*D/fabs(CV));
	   RD=short(vleft*D/fabs(CV));
   }

	//设置发送数据
   byteBuf[0]=CMD_VELOCITY_DISTANCE;	
   
   pvel=&vleft;
   pbyte=(BYTE*)pvel;
   byteBuf[3]=*pbyte++;		
   byteBuf[4]=*pbyte++;		
   byteBuf[1]=*pbyte++;		
   byteBuf[2]=*pbyte;
   
   pvel=&vright;
   pbyte=(BYTE*)pvel;
   byteBuf[7]=*pbyte++;		
   byteBuf[8]=*pbyte++;		
   byteBuf[5]=*pbyte++;		
   byteBuf[6]=*pbyte;
   
   ppos=&LD;
   pbyte=(BYTE*)ppos;		
   byteBuf[9]=*pbyte++;		
   byteBuf[10]=*pbyte;
   
   ppos=&RD;
   pbyte=(BYTE*)ppos;		
   byteBuf[11]=*pbyte++;		
   byteBuf[12]=*pbyte;
   
   pflag=&returnflag;
   pbyte=(BYTE*)pflag;
   byteBuf[13]=*pbyte++;		
   byteBuf[14]=*pbyte;
   
   //发送
   OutBuf.SetSize(15); 
   m_comm->SetRThreshold(9);
   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
   m_comm->SetOutput(COleVariant(OutBuf)); 
   
   return;
	
	return;

}
void CDrive::drive_forward_position2(CMSComm* m_comm, float V,float Omega,short Distance,bool FinishReturn)
{
	float vleft,vright;//左右轮速度
	short D;
	double w;

	short LD,RD;
	short returnflag;
	   
	if(FinishReturn)
	{
		returnflag=1;
	}
	else
	{
		returnflag=0;
	}
	   
	//输入速度和距离处理
	D=Distance;
	if(D<0)
	{
		D=0;
	}
    //输入速度处理
	if(V<0)
	{
		vleft=0;
		vright=0;
	}
	else
	{
		vleft=float(V+ROBOTW*(Omega*PI/180)/2);
		vright=float(V-ROBOTW*(Omega*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//左右轮速度   cm/s->r/m
	vleft=(float) (60*vleft/(PI*WHEELD));
	vright=(float) (60*vright/(PI*WHEELD));
   
	float CV;
	CV=(vleft+vright)/2;
	if(CV==0)
	{
		LD=0;
		RD=0;	   
	}
	else
	{
		LD=short(vleft*D/fabs(CV));
		RD=short(vleft*D/fabs(CV));
	}
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;
	   
	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;
	   
	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_back_distance(CMSComm* m_comm, float AverageV,short Distance,bool FinishReturn)
{
	float V,vleft,vright;//平均速度，左右轮速度
	short D;
	short LD,RD;
	short returnflag;
	   
	if(FinishReturn)
	{
		returnflag=1;
	}
	else
	{
		returnflag=0;
	}
	   
	   //输入速度和距离处理
	   D=Distance;
	   if(D<0)
	   {
		   D=0;
	   }
	   
	   V=AverageV;
	   if(V<0)
	   {
		   V=0;
	   }
	   if(V>MAXVELOCITY)
	   {
		   V=MAXVELOCITY;
	   }
	   
	   //计算左右轮速度
	   V=(float) (60*V/(PI*WHEELD)); //cm/s->r/m
	   vleft=-V;
	   vright=-V;

	  float CV;
	  CV=(vleft+vright)/2;
	  if(CV==0)
	  {
		  LD=0;
		  RD=0;	   
	  }
	  else
	  {
		  LD=short(vleft*D/fabs(CV));
		  RD=short(vleft*D/fabs(CV));
	  }
	   
	   //设置发送数据
	   byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;
	   
	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;
	   
	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_back_position1(CMSComm* m_comm, float AverageV,float VAngle,short Distance,bool FinishReturn)
{
	float vleft,vright;//左右轮速度
	short D;
	double w;
	short LD,RD;
	short returnflag;
	   
	if(FinishReturn)
	{
		returnflag=1;
	}
	else
	{
		returnflag=0;
	}
	   
	//输入速度和距离处理
	D=Distance;
	if(D<0)
	{
		D=0;
	}

    if(AverageV<0)
	{
		vleft=0;
		vright=0;
	}
    else
	{
		vleft=float(AverageV-ROBOTW*tan(VAngle*PI/180)/2);
		vright=float(AverageV+ROBOTW*tan(VAngle*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	
	//左右轮速度   cm/s->r/m
	vleft=(float) (-60*vleft/(PI*WHEELD));
	vright=(float) (-60*vright/(PI*WHEELD));
	
	   float CV;
	   CV=(vleft+vright)/2;
	   if(CV==0)
	   {
		   LD=0;
		   RD=0;	   
	   }
	   else
	   {
		   LD=short(vleft*D/fabs(CV));
		   RD=short(vleft*D/fabs(CV));
	   }
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;
	   
	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;
	   
	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_back_position2(CMSComm* m_comm, float V,float Omega,short Distance,bool FinishReturn)
{
	float vleft,vright;//左右轮速度
	short D;
	double w;
	short LD,RD;
	short returnflag;
	   
	if(FinishReturn)
	{
		returnflag=1;
	}
	else
	{
		returnflag=0;
	}
	   
	//输入速度和距离处理
	D=Distance;
	if(D<0)
	{
		D=0;
	}
    if(V<0)
	{
		vleft=0;
		vright=0;
	}
	else
	{
		vleft=float(V-ROBOTW*(Omega*PI/180)/2);
		vright=float(V+ROBOTW*(Omega*PI/180)/2);
		if(vleft<-MAXVELOCITY)
		{
			vleft=-MAXVELOCITY;
		}
		else if(vleft>MAXVELOCITY)
		{
			vleft=MAXVELOCITY;
		}
		if(vright<-MAXVELOCITY)
		{
			vright=-MAXVELOCITY;
		}
		else if(vright>MAXVELOCITY)
		{
			vright=MAXVELOCITY;
		}
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//左右轮速度   cm/s->r/m
	vleft=(float) (-60*vleft/(PI*WHEELD));
	vright=(float) (-60*vright/(PI*WHEELD));
	
	   float CV;
	   CV=(vleft+vright)/2;
	   if(CV==0)
	   {
		   LD=0;
		   RD=0;	   
	   }
	   else
	   {
		   LD=short(vleft*D/fabs(CV));
		   RD=short(vleft*D/fabs(CV));
	   }
	
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;
	   
	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;
	   
	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_move_position(CMSComm* m_comm, float LeftV,float RightV,short Distance,bool FinishReturn)
{
	float vleft,vright;//左右轮速度
	short D;
	double w;
	short LD,RD;
	short returnflag;
	   
	if(FinishReturn)
	{
		returnflag=1;
	}
	else
	{
		returnflag=0;
	}
	   
	//输入速度和距离处理
	D=Distance;
	if(D<0)
	{
		D=0;
	}
    vleft=LeftV;
	vright=RightV;
	
	if(vleft>MAXVELOCITY)
	{
		vleft=MAXVELOCITY;
	}
	else if(vleft<-MAXVELOCITY)
	{
		vleft=-MAXVELOCITY;
	}
	
	if(vright>MAXVELOCITY)
	{
		vright=MAXVELOCITY;
	}
	else if(vright<-MAXVELOCITY)
	{
		vright=-MAXVELOCITY;
	}
	
	w=(vleft-vright)*180/(ROBOTW*PI);
	if(w>MAXOMEGA)
	{
		vleft=ROTATEVELOCITY;
		vright=-ROTATEVELOCITY;		
	}
	else if(w<-MAXOMEGA)
	{
		vleft=-ROTATEVELOCITY;
		vright=ROTATEVELOCITY;	
	}
	
	//计算左右轮速度   cm/s->r/m
	vleft=(float) (60*vleft/(PI*WHEELD));
	vright=(float) (60*vright/(PI*WHEELD));

	   float CV;
	   CV=(vleft+vright)/2;
	   if(CV==0)
	   {
		   LD=0;
		   RD=0;	   
	   }
	   else
	   {
		   LD=short(vleft*D/fabs(CV));
		   RD=short(vleft*D/fabs(CV));
	   }
	
	//设置发送数据
	byteBuf[0]=CMD_VELOCITY_DISTANCE;	
	   
	   pvel=&vleft;
	   pbyte=(BYTE*)pvel;
	   byteBuf[3]=*pbyte++;		
	   byteBuf[4]=*pbyte++;		
	   byteBuf[1]=*pbyte++;		
	   byteBuf[2]=*pbyte;
	   
	   pvel=&vright;
	   pbyte=(BYTE*)pvel;
	   byteBuf[7]=*pbyte++;		
	   byteBuf[8]=*pbyte++;		
	   byteBuf[5]=*pbyte++;		
	   byteBuf[6]=*pbyte;
	   
	   ppos=&LD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[9]=*pbyte++;		
	   byteBuf[10]=*pbyte;
	   
	   ppos=&RD;
	   pbyte=(BYTE*)ppos;		
	   byteBuf[11]=*pbyte++;		
	   byteBuf[12]=*pbyte;
	   
	   pflag=&returnflag;
	   pbyte=(BYTE*)pflag;
	   byteBuf[13]=*pbyte++;		
	   byteBuf[14]=*pbyte;
	   
	   //发送
	   OutBuf.SetSize(15); 
	   m_comm->SetRThreshold(9);
	   for(i=0;i<15;i++)OutBuf[i] = byteBuf[i]; 
	   m_comm->SetOutput(COleVariant(OutBuf)); 
	   
	   return;
}

void CDrive::drive_status_query(CMSComm* m_comm)
{
	OutBuf.SetSize(1);
	OutBuf[0] = CMD_STATUS_QUERY; 
	m_comm->SetRThreshold(9);	
	m_comm->SetOutput(COleVariant(OutBuf)); 
}
void CDrive::drive_set_startpoint(CMSComm* m_comm)
{
	OutBuf.SetSize(1);
	OutBuf[0] = CMD_SET_STARTPOINT;	
	m_comm->SetOutput(COleVariant(OutBuf)); 
}
void CDrive::drive_local_distance_query(CMSComm* m_comm)
{
	OutBuf.SetSize(1);
	OutBuf[0] = CMD_LOCAL_DISTANCE_QUERY; 
	m_comm->SetRThreshold(9);	
	m_comm->SetOutput(COleVariant(OutBuf)); 
}
void CDrive::drive_global_distance_query(CMSComm* m_comm)
{
	OutBuf.SetSize(1);
	OutBuf[0] = CMD_GLOBAL_DISTANCE_QUERY; 
	m_comm->SetRThreshold(9);	
	m_comm->SetOutput(COleVariant(OutBuf)); 
}

void CDrive::drive_position_reset(CMSComm* m_comm)
{
	OutBuf.SetSize(1);
	OutBuf[0] = CMD_POS_RESET; 	
	m_comm->SetOutput(COleVariant(OutBuf)); 
}







