// g_outline.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include <opencv2\opencv.hpp>
#include"outline.h"
#include<windows.h>
 
using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string imagename = "D:\\new\\matlab\\cellexp2\\exp3.jpg";   //�˴���Ҫ��д���Ե�ַ���Ҳ���ʱʹ����Ե�ַ����
    DWORD dwStartTime = GetTickCount();
	//����ͼ��
	Mat img = imread(imagename);
	int d = img.depth();
    Mat contours,img1,img2;
	//�������ͼ��ʧ��
	if(img.empty())
	{
		return -1;
	}
 
	//��������
	namedWindow("image",1);
    cvtColor(img,img,CV_RGB2GRAY);
	
	outline out;
	out.Get_img(img);
	contours = out.nfilter();
	
	//��ʾͼ��
	cout<<"��ʱ"<<GetTickCount()-dwStartTime<<"ms"<<endl;
	imshow("image", contours);
 
	//�ȴ����������������������
	waitKey();
 
	return 0;
 
       //����ָ��: feihongmeilian
       //��������֤��string imagename = "lena.jpg";  ���ǿ���ʹ����Ե�ַ������ʹ�û���ΪOpenCV 2.4.3
       //ͼƬΪmeinv.jpg���ڴ����ļ������ļ��е���һ���ļ����У�string imagename = "..\\meinv.jpg"  ͼƬ��������ʾ��
      //������ע����� ˫���š��� �г��ֵĵ�һ�� б�� \ ������ת���ַ�
}

