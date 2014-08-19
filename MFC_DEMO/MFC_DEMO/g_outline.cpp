// g_outline.cpp : 定义控制台应用程序的入口点。
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
	string imagename = "D:\\new\\matlab\\cellexp2\\exp3.jpg";   //此处需要填写绝对地址，我测试时使用相对地址出错。
    DWORD dwStartTime = GetTickCount();
	//读入图像
	Mat img = imread(imagename);
	int d = img.depth();
    Mat contours,img1,img2;
	//如果读入图像失败
	if(img.empty())
	{
		return -1;
	}
 
	//创建窗口
	namedWindow("image",1);
    cvtColor(img,img,CV_RGB2GRAY);
	
	outline out;
	out.Get_img(img);
	contours = out.nfilter();
	
	//显示图像
	cout<<"耗时"<<GetTickCount()-dwStartTime<<"ms"<<endl;
	imshow("image", contours);
 
	//等待按键，按键盘任意键返回
	waitKey();
 
	return 0;
 
       //错误指出: feihongmeilian
       //经本人验证，string imagename = "lena.jpg";  处是可以使用相对地址，本人使用环境为OpenCV 2.4.3
       //图片为meinv.jpg置于代码文件所在文件夹的上一级文件夹中，string imagename = "..\\meinv.jpg"  图片可正常显示。
      //需提醒注意的是 双引号“” 中出现的第一个 斜杠 \ 被视作转义字符
}

