
// MFC_DEMODlg.h : ͷ�ļ�
//

#pragma once
#include<opencv2\opencv.hpp>
#include"CvvImage.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include"outline.h"
#include "Drive.h"
#include "mscomm.h"
#include "afxwin.h"
// CMFC_DEMODlg �Ի���
class CMFC_DEMODlg : public CDialogEx
{
// ����
public:
	CMFC_DEMODlg(CWnd* pParent = NULL);	// ��׼���캯��
	UINT  m_driveport;
	short m_acceleration;
	CDrive m_drive;
	short  statusquery;
	
// Dialog Data
	//{{AFX_DATA(CNewDriveTestDlg)
	
// �Ի�������
	enum { IDD = IDD_MFC_DEMO_DIALOG };
	int		m_leftglobaldistance;
	int		m_leftlocaldistance;
	int		m_leftvelocity;
	int		m_rightglobaldistance;
	int		m_rightlocaldistance;
	int		m_rightvelocity;
	int		m_rvelocity;
	int		m_tvelocity;
	int		m_velocityangle;
	//CMSComm	m_drivecomm;
	int		m_averagevelocity;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	IplImage *Image_Source ,*ImageResult,*ImageToRecogGray;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenImage();
	void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedGetContours();
	cv::Mat  GetContours(IplImage *img);
	void ShowImage(IplImage *ImageToShow,int ID_Number,BITMAPINFO *BMP);
private:
	BYTE *pImageBuffer;//�ɼ�ͼ�����ݻ�����
	BYTE *pImageBuffer2;//bayerתRGBͼ�񻺳���
	BITMAPINFO *m_pSrcBmpInfo;		//BITMAPINFO �ṹָ�룬��ʾ����ǰ��ͼ��ʱʹ��
	BITMAPINFO *m_pObsBmpInfo;		//BITMAPINFO �ṹָ�룬��ʾ�ϰ����ͼʱʹ��
	CRect SrRImageRect;             //Դͼ����
	bool status,Image_Start;
	GX_DEV_HANDLE m_hDevice;//USB ������������

public:
	CMSComm m_comm;
	DECLARE_EVENTSINK_MAP()
	void OnDriveCommMscomm();
	
};
