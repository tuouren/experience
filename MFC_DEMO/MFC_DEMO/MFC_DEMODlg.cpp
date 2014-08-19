
// MFC_DEMODlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_DEMODlg �Ի���




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


// CMFC_DEMODlg ��Ϣ�������

BOOL CMFC_DEMODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	ShowWindow(SW_MAXIMIZE);

	statusquery=0;
	m_driveport=2;
	m_acceleration=3;
	m_drive.drive_init(&m_comm, m_driveport,m_acceleration);
	//m_drive.drive_forward_line(&m_comm,20);
	Image_Source = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,3);
	ImageResult = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,3);
	ImageToRecogGray = cvCreateImage(cvSize(WIMG,HIMG),IPL_DEPTH_8U,1);// TODO: �ڴ���Ӷ���ĳ�ʼ������
	Image_Start = false;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_DEMODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_DEMODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_DEMODlg::OnBnClickedOpenImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//IplImage *image=NULL; //ԭʼͼ��
    //if(image) cvReleaseImage(&image);
	Image_Start=true;
	pImageBuffer = new BYTE[640 * 512 ];
	pImageBuffer2 = new BYTE[640 * 512 *3 ];
	GXInitLib(); //��ʼ����
	uint32_t m_nNumberDevice;
	GXUpdateDeviceList(&m_nNumberDevice,1000);//����豸����
	GX_DEVICE_BASE_INFO *baseinfo = new GX_DEVICE_BASE_INFO[m_nNumberDevice];
	size_t nSize = m_nNumberDevice * sizeof(GX_DEVICE_BASE_INFO);//��ȡ�豸��Ϣ
	status = GXGetAllDeviceBaseInfo(baseinfo, &nSize);
	status =GXOpenDeviceByIndex(1, &m_hDevice);//�����������ע���Ѿ�����Ĭ�ϲ�������

	status = GXSetInt(m_hDevice, GX_INT_DECIMATION_HORIZONTAL , 2);//ˮƽ��ȡ���߶ȷֱ��ʱ�Ϊ512��Ĭ��1024��
	status = GXSetInt(m_hDevice, GX_INT_DECIMATION_VERTICAL , 2);//��ֱ��ȡ����ȷֱ��ʱ�Ϊ640��Ĭ��1280��

	status =GXSetInt(m_hDevice,GX_INT_WIDTH,640);//����ͼ����640
	status =GXSetInt(m_hDevice,GX_INT_HEIGHT,512);//����ͼ��߶�512
	status =GXSetEnum(m_hDevice,GX_ENUM_ACQUISITION_MODE,GX_ACQ_MODE_SINGLE_FRAME);//���òɼ�ģʽΪ��֡�ɼ�
	status =GXSetInt(m_hDevice,GX_INT_ACQUISITION_SPEED_LEVEL,12);//���òɼ��ٶȣ���Χ(0-12)
	status =GXSetInt(m_hDevice,GX_INT_GAIN,60);//��������,���淶Χ(0-63)
	status =GXSetInt(m_hDevice,GX_INT_GAIN,GX_ENUM_GAIN_AUTO);//��������,���淶Χ(0-63)
	status =GXSetFloat(m_hDevice,GX_FLOAT_EXPOSURE_TIME,25000);//�ع�ʱ��30ms
 	status = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_CONTINUOUS);//���������Զ���ƽ��

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
	//cvNamedWindow( "Image", 1 );//��������
   // cvShowImage( "Image", &contours );
    
	//�ȴ����������������������
//	waitKey();
	DrawPicToHDC(Image_Source, IDC_STATIC2);
	DrawPicToHDC(&contours, IDC_STATIC);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ShowImage(Image_Source,IDC_STATIC,m_pSrcBmpInfo);
	//DrawPicToHDC(Image_Source, IDC_STATIC);
	


	CDialog::OnTimer(nIDEvent);
}
void CMFC_DEMODlg::ShowImage(IplImage *ImageToShow,int ID_Number,BITMAPINFO *BMP)//�ɼ�����ʾͼ��
{
	int flag;
	CClientDC ShowDC(GetDlgItem(ID_Number));
	::SetStretchBltMode(ShowDC.GetSafeHdc(),COLORONCOLOR);
	flag=StretchDIBits(ShowDC.GetSafeHdc(),//
		0,						
		0,
		ImageToShow->width,	    //��ʾ���ڿ��
		ImageToShow->height,    	//��ʾ���ڸ߶�
		0,
		0,
		ImageToShow->width,					//ͼ����
		ImageToShow->height,					//ͼ��߶�
		ImageToShow->imageData,			//ͼ�񻺳���
		BMP,				//BMPͼ��������Ϣ
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
    cimg.CopyOf( img ); // ����ͼƬ
    cimg.DrawToHDC( hDC, &rect ); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
    ReleaseDC( pDC );

	
}

void CMFC_DEMODlg::OnBnClickedGetContours()
{
	cv::Mat contours1 = GetContours(this->Image_Source);
	IplImage  contours = contours1;
	//cvNamedWindow( "Image", 1 );//��������
   // cvShowImage( "Image", &contours );
    
	//�ȴ����������������������
//	waitKey();
	DrawPicToHDC(&contours, IDC_STATIC);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	if(m_comm.GetCommEvent()==2)  //�����¼�Ϊ2��ʾ���յ��ַ�   
	{
		variant_inp=m_comm.GetInput(); //�������� 
		safearray_inp=variant_inp;        //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���   
		len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
		
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������
		
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
    UpdateData(false);	// TODO: �ڴ˴������Ϣ����������
}


