/***********************************************************************/
/*  Project         -     Robot driver                                           */
/*  File Name       -     drive.h                                                */
/*  Purpose         -     Define variables and functions                         */
/*  Author          -     Liu Xianhua                                            */
/*  Date            -     2008/03/13                                              */
/*  Version         -     1.0                                                    */
/*********************************************************************************/
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DRIVE_H__20A9E040_8940_4FB8_8A1E_9F91CDAA4EAE__INCLUDED_)
#define AFX_DRIVE_H__20A9E040_8940_4FB8_8A1E_9F91CDAA4EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Drive.h : header file
//
#include "mscomm.h"

#define MAXVELOCITY                100 /*����ٶ�cm/s*/
#define MAXOMEGA                   100  /*�����ٶ�degree/s*/
#define PI                         3.1415926
#define ROBOTW                     27.1 
#define WHEELD                     21.0
#define ROTATEVELOCITY             float(MAXOMEGA*PI*ROBOTW*0.5/180)  /*ԭ����ת�ٶ�*/                   


//ָ�����Ͷ���
#define CMD_INIT                    0x0015//��ʼ��ָ��
#define CMD_VELOCITY                0x0002//�ٶ�ָ��
#define CMD_VELOCITY_ANGLE          0x0004//��תָ��
#define CMD_VELOCITY_DISTANCE       0x0005//�˶�һ������ָ��
#define CMD_STATUS_QUERY            0x0007//״̬��ѯָ��
#define CMD_SET_STARTPOINT          0x0013//����λ����ʼ��
#define CMD_LOCAL_DISTANCE_QUERY    0x0014//��ѯλ����ʼ�㵽��ǰʱ���˶�����
#define CMD_GLOBAL_DISTANCE_QUERY   0x000D//��ѯ����������ǰʱ���˶�����
#define CMD_FLAG_QUERY              0x000A//��λ�������˶�ָ���Ƿ���ɱ�־
#define CMD_POS_RESET               0x000E//���ȫ�����̼���

/////////////////////////////////////////////////////////////////////////////
// CDrive window

class CDrive : public CWnd
{
// Construction
public:
	CDrive();

// Attributes
public:
	int i;
	BYTE byteBuf[20];  
	float *pvel;		
	short *ppos;
	short  *pflag;
	BYTE  *pbyte;
	CByteArray OutBuf; 
	CString str;

// Operations
public:
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
   void drive_position_reset(CMSComm* m_comm);

	     

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrive)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrive();
    
	// Generated message map functions
protected:
	//{{AFX_MSG(CDrive)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVE_H__20A9E040_8940_4FB8_8A1E_9F91CDAA4EAE__INCLUDED_)










































