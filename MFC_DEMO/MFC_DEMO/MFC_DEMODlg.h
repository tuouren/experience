
// MFC_DEMODlg.h : 头文件
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
// CMFC_DEMODlg 对话框
class CMFC_DEMODlg : public CDialogEx
{
// 构造
public:
	CMFC_DEMODlg(CWnd* pParent = NULL);	// 标准构造函数
	UINT  m_driveport;
	short m_acceleration;
	CDrive m_drive;
	short  statusquery;
	
// Dialog Data
	//{{AFX_DATA(CNewDriveTestDlg)
	
// 对话框数据
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	IplImage *Image_Source ,*ImageResult,*ImageToRecogGray;
	// 生成的消息映射函数
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
	BYTE *pImageBuffer;//采集图像数据缓冲区
	BYTE *pImageBuffer2;//bayer转RGB图像缓冲区
	BITMAPINFO *m_pSrcBmpInfo;		//BITMAPINFO 结构指针，显示处理前的图像时使用
	BITMAPINFO *m_pObsBmpInfo;		//BITMAPINFO 结构指针，显示障碍物地图时使用
	CRect SrRImageRect;             //源图区域
	bool status,Image_Start;
	GX_DEV_HANDLE m_hDevice;//USB 数字摄像机句柄

public:
	CMSComm m_comm;
	DECLARE_EVENTSINK_MAP()
	void OnDriveCommMscomm();
	
};
