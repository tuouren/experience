
// MFC_DEMODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_DEMO.h"
#include "MFC_DEMODlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int HIMG=512;
const int WIMG=640;
const int SIMG=HIMG*WIMG;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CMFC_DEMODlg 对话框




CMFC_DEMODlg::CMFC_DEMODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_DEMODlg::IDD, pParent)
{
	m_leftglobaldistance = 0;
	m_leftlocaldistance = 0;
	m_leftvelocity = 0;
	m_rightglobaldistance = 0;
	m_rightlocaldistance = 0;
	m_rightvelocity = 0;
	m_rvelocity = 0;
	m_tvelocity = 0;
	m_velocityangle = 0;
	m_averagevelocity = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_DEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	DDX_Text(pDX, IDC_LEFTGLOBALDISTANCE, m_leftglobaldistance);
	DDX_Text(pDX, IDC_LEFTLOCALDISTANCE, m_leftlocaldistance);
	DDX_Text(pDX, IDC_LEFTVELOCITY, m_leftvelocity);
	DDX_Text(pDX, IDC_RIGHTGLOBALDISTANCE, m_rightglobaldistance);
	DDX_Text(pDX, IDC_RIGHTLOCALDISTANCE, m_rightlocaldistance);
	DDX_Text(pDX, IDC_RIGHTVELOCITY, m_rightvelocity);
	DDX_Text(pDX, IDC_RVELOCITY, m_rvelocity);
	DDX_Text(pDX, IDC_TVELOCITY, m_tvelocity);
	DDX_Text(pDX, IDC_VELOCITYANGLE, m_velocityangle);
	DDX_Text(pDX, IDC_AVERAGEVELOCITY, m_averagevelocity);
}

BEGIN_MESSAGE_MAP(CMFC_DEMODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_DEMODlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC_DEMODlg::OnBnClickedGetContours)
	END_MESSAGE_MAP()


// CMFC_DEMODlg 消息处理程序

BOOL CMFC_DEMODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	ShowWindow(SW_MAXIMIZE);

	statusquery=0;
	m_driveport=2;
	m_acceleration=3;
	m_drive.drive_init(&m_comm, m_driveport,m_acceleration);
	//m_drive.drive_forward_line(&m_comm,20);
	Image_Source = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,3);
	ImageResult = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,3);
	ImageToRecogGray = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,1);// TODO: 在此添加额外的初始化代码
	Image_Start = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_DEMODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_DEMODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_DEMODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_DEMODlg::OnBnClickedOpenImage()
{
	// TODO: 在此添加控件通知处理程序代码
	//IplImage *image=NULL; //原始图像
    //if(image) cvReleaseImage(&image);
	Image_Start=true;
	pImageBuffer = new BYTE[640 * 512 ];
	pImageBuffer2 = new BYTE[640 * 512 *3 ];
	GXInitLib(); //初始化库
	uint32_t m_nNumberDevice;
	GXUpdateDeviceList(&m_nNumberDevice,1000);//获得设备个数
	GX_DEVICE_BASE_INFO *baseinfo = new GX_DEVICE_BASE_INFO[m_nNumberDevice];
	size_t nSize = m_nNumberDevice * sizeof(GX_DEVICE_BASE_INFO);//获取设备信息
	status = GXGetAllDeviceBaseInfo(baseinfo, &nSize);
	status =GXOpenDeviceByIndex(1, &m_hDevice);//打开数字相机，注：已经包含默认参数设置

	status = GXSetInt(m_hDevice, GX_INT_DECIMATION_HORIZONTAL , 2);//水平抽取，高度分辨率变为512（默认1024）
	status = GXSetInt(m_hDevice, GX_INT_DECIMATION_VERTICAL , 2);//垂直抽取，宽度分辨率变为640（默认1280）

	status =GXSetInt(m_hDevice,GX_INT_WIDTH,640);//设置图像宽度640
	status =GXSetInt(m_hDevice,GX_INT_HEIGHT,512);//设置图像高度512
	status =GXSetEnum(m_hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);//设置采集模式为单帧采集
	status =GXSetInt(m_hDevice,GX_INT_ACQUISITION_SPEED_LEVEL,12);//设置采集速度，范围(0-12)
	status =GXSetInt(m_hDevice,GX_INT_GAIN,60);//设置增益,增益范围(0-63)
	status =GXSetInt(m_hDevice,GX_INT_GAIN,GX_ENUM_GAIN_AUTO);//设置增益,增益范围(0-63)
	status =GXSetFloat(m_hDevice,GX_FLOAT_EXPOSURE_TIME,25000);//曝光时间30ms
 	status = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_CONTINUOUS);//开启连续自动白平衡

	m_pSrcBmpInfo=(BITMAPINFO *)new BYTE[40];
	m_pSrcBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
    m_pSrcBmpInfo->bmiHeader.biWidth		= WIMG;		
	m_pSrcBmpInfo->bmiHeader.biHeight		= HIMG;		
    m_pSrcBmpInfo->bmiHeader.biBitCount		= 24;
	
	m_pSrcBmpInfo->bmiHeader.biPlanes			= 1;	
	m_pSrcBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pSrcBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pSrcBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pSrcBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
    m_pSrcBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pSrcBmpInfo->bmiHeader.biClrImportant	    = 0;

	m_pObsBmpInfo=(BITMAPINFO *)new BYTE[40];
	m_pObsBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
    m_pObsBmpInfo->bmiHeader.biWidth		= WIMG;		
	m_pObsBmpInfo->bmiHeader.biHeight		= HIMG;		
    m_pObsBmpInfo->bmiHeader.biBitCount		= 24;
	
	m_pObsBmpInfo->bmiHeader.biPlanes			= 1;	
	m_pObsBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pObsBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pObsBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pObsBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
    m_pObsBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pObsBmpInfo->bmiHeader.biClrImportant	    = 0;



    SetTimer(1,320,NULL);
    //DrawPicToHDC(image, IDC_STATIC);
}
void CMFC_DEMODlg::OnTimer(UINT nIDEvent){
	
	if(statusquery==0)
	{
		m_drive.drive_status_query(&m_comm);
		statusquery=1;
	}
	else if(statusquery==1)
	{
		m_drive.drive_local_distance_query(&m_comm);
		statusquery=2;
	}
	else
	{
		m_drive.drive_global_distance_query(&m_comm);
		statusquery=0;
	}
	GX_FRAME_DATA frameData;
	frameData.pImgBuf = pImageBuffer;
	frameData.nStatus = -1;
	status = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	do
	{
		status = GXGetImage(m_hDevice, &frameData, 50); 
	} 
	while(frameData.nStatus != 0);
	int64_t m_nBayer;
	GXGetEnum(m_hDevice,GX_ENUM_PIXEL_COLOR_FILTER,&m_nBayer);
	status = DxRaw8toRGB24(pImageBuffer,pImageBuffer2,WIMG,HIMG,RAW2RGB_NEIGHBOUR,DX_PIXEL_COLOR_FILTER(m_nBayer),TRUE);
	status = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
	Image_Source->imageData=(char*)pImageBuffer2;
	cv::Mat contours1 = GetContours(this->Image_Source);
	IplImage  contours = contours1;
	//cvNamedWindow( "Image", 1 );//创建窗口
   // cvShowImage( "Image", &contours );
    
	//等待按键，按键盘任意键返回
//	waitKey();
	DrawPicToHDC(Image_Source, IDC_STATIC2);
	DrawPicToHDC(&contours, IDC_STATIC);
	// TODO: 在此添加控件通知处理程序代码
	//ShowImage(Image_Source,IDC_STATIC,m_pSrcBmpInfo);
	//DrawPicToHDC(Image_Source, IDC_STATIC);
	


	CDialog::OnTimer(nIDEvent);
}
void CMFC_DEMODlg::ShowImage(IplImage *ImageToShow,int ID_Number,BITMAPINFO *BMP)//采集并显示图像
{
	int flag;
	CClientDC ShowDC(GetDlgItem(ID_Number));
	::SetStretchBltMode(ShowDC.GetSafeHdc(),COLORONCOLOR);
	flag=StretchDIBits(ShowDC.GetSafeHdc(),//
		0,						
		0,
		ImageToShow->width,	    //显示窗口宽度
		ImageToShow->height,    	//显示窗口高度
		0,
		0,
		ImageToShow->width,					//图像宽度
		ImageToShow->height,					//图像高度
		ImageToShow->imageData,			//图像缓冲区
		BMP,				//BMP图像描述信息
		DIB_RGB_COLORS,
		SRCCOPY
		);
}

void CMFC_DEMODlg::DrawPicToHDC(IplImage *img, UINT ID){
	CDC *pDC = GetDlgItem(ID)->GetDC();
    HDC hDC= pDC->GetSafeHdc();
    CRect rect;
    GetDlgItem(ID)->GetClientRect(&rect);
    CvvImage cimg;
    cimg.CopyOf( img ); // 复制图片
    cimg.DrawToHDC( hDC, &rect ); // 将图片绘制到显示控件的指定区域内
    ReleaseDC( pDC );

	
}

void CMFC_DEMODlg::OnBnClickedGetContours()
{
	cv::Mat contours1 = GetContours(this->Image_Source);
	IplImage  contours = contours1;
	//cvNamedWindow( "Image", 1 );//创建窗口
   // cvShowImage( "Image", &contours );
    
	//等待按键，按键盘任意键返回
//	waitKey();
	DrawPicToHDC(&contours, IDC_STATIC);
	// TODO: 在此添加控件通知处理程序代码
}

cv::Mat  CMFC_DEMODlg::GetContours(IplImage *img){
	cv::Mat image(img,false); 
	cv::cvtColor(image,image,CV_RGB2GRAY);
	outline out_line;
	
	out_line.Get_img(image);
	return out_line.nfilter();
	
}

BEGIN_EVENTSINK_MAP(CMFC_DEMODlg, CDialogEx)
	ON_EVENT(CMFC_DEMODlg, IDC_MSCOMM1, 1, CMFC_DEMODlg::OnDriveCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()


void CMFC_DEMODlg::OnDriveCommMscomm()
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	int temp=0;
	BYTE rxdata[2048]; 	
	short *leftvelocity;
	short *rightvelcoity;
	float *localleftdistance;
	float *localrightdistance;
	float *globalleftdistance;
	float *globalrightdistance;
	short  *iffinished;
	short  *cmdtype;

	if(m_comm.GetCommEvent()==2)  //串口事件为2表示接收到字符   
	{
		variant_inp=m_comm.GetInput(); //读缓冲区 
		safearray_inp=variant_inp;        //VARIANT型变量转换为ColeSafeArray型变量   
		len=safearray_inp.GetOneDimSize(); //得到有效数据长度
		
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组
		
		BYTE mydata[12];
		mydata[1]=0;
		mydata[2]=0;
		mydata[3]=0;
		mydata[4]=0;
		
		mydata[5]=0;
		mydata[6]=0;
		mydata[7]=0;
		mydata[8]=0;

		mydata[9]=0;
		mydata[10]=0;

		mydata[0]=rxdata[0];
		switch (mydata[0])
        {
		case CMD_STATUS_QUERY:
			
			mydata[1]=rxdata[2];
			mydata[2]=rxdata[1];

			mydata[3]=rxdata[4];
			mydata[4]=rxdata[3];
			
			mydata[5]=rxdata[6];
			mydata[6]=rxdata[5];

			mydata[7]=rxdata[8];
			mydata[8]=rxdata[7];

			leftvelocity=(short *)(&(mydata[1]));
			rightvelcoity=(short *)(&(mydata[3])); 
			cmdtype=(short *)(&(mydata[5]));
			iffinished=(short *)(&(mydata[7]));
			m_leftvelocity=(int)(*leftvelocity)*WHEELD*PI/60;
            m_rightvelocity=(int)(*rightvelcoity)*WHEELD*PI/60;
			m_averagevelocity=(m_leftvelocity+m_rightvelocity)/2;
			m_velocityangle=(atan((m_leftvelocity-m_rightvelocity)/ROBOTW)*180/PI);
			m_tvelocity=(m_leftvelocity+m_rightvelocity)/2;
			m_rvelocity=(((m_leftvelocity-m_rightvelocity)/ROBOTW)*180/PI);
			break;

		case CMD_LOCAL_DISTANCE_QUERY:
			mydata[1]=rxdata[2];
			mydata[2]=rxdata[1];
			mydata[3]=rxdata[4];
			mydata[4]=rxdata[3];
			
			mydata[5]=rxdata[6];
			mydata[6]=rxdata[5];
			mydata[7]=rxdata[8];
			mydata[8]=rxdata[7];

			localleftdistance=(float *)(&(mydata[1]));
			localrightdistance=(float *)(&(mydata[5]));
			m_leftlocaldistance=int(*localleftdistance);
			m_rightlocaldistance=int(*localrightdistance);
			break;

		case CMD_GLOBAL_DISTANCE_QUERY:
			mydata[1]=rxdata[2];
			mydata[2]=rxdata[1];
			mydata[3]=rxdata[4];
			mydata[4]=rxdata[3];
			
			mydata[5]=rxdata[6];
			mydata[6]=rxdata[5];
			mydata[7]=rxdata[8];
			mydata[8]=rxdata[7];
			
			globalleftdistance=(float *)(&(mydata[1]));
			globalrightdistance=(float *)(&(mydata[5]));
			m_leftglobaldistance=int(*globalleftdistance);
			m_rightglobaldistance=int(*globalrightdistance);
			break;
		case CMD_FLAG_QUERY:
            MessageBox("finished");
            break;
		default:
			break;
        }
		
	}
    UpdateData(false);	// TODO: 在此处添加消息处理程序代码
}


