#include "stdafx.h"
#include "outline.h"
#include <opencv2\opencv.hpp>

int bwlabel(IplImage* img, int n, int* labels);

using namespace cv;
using namespace std;



outline::outline(){

}
void outline::Get_img(Mat &img){
		this->img = img;
	}
int outline::Get_HighThresh(){
    MatND hist;   
	float hranges[2] ;
	int histSize[1] ;
	int channels[1];
	const float* ranges[1];
	histSize[0] = 256;
	hranges[0] = 0.0;
	hranges[1] = 255.0;
	channels[0] = 0;
	ranges[0] = hranges;
	
	calcHist(&this->img,1,&channels[0],cv::Mat(),hist,1,histSize,ranges);
	float sum = 0.0f;
	for(int i = 0;i<256;i++){
	  sum = sum+hist.at<float>(i);
	  if(sum>0.7*this->img.rows*this->img.cols)return this->HighThresh=i;
	}
}
Mat outline::nfilter(){
	 Mat grad_x,grad_y;
	 Mat norm,dir;
	 Mat sobel_img;
	vector<int> vec_lab;
	 vector<vector<Point> > contours,contours1;
	 Sobel( this->img, grad_x,CV_16S,1, 0);
	 Sobel( this->img, grad_y,CV_16S,0, 1);
	 //magnitude(grad_x,grad_y,sobel_img);
	 sobel_img  = abs(grad_x)+abs(grad_y);
	 sobel_img.convertTo(sobel_img,CV_8U);
	 threshold(sobel_img,norm,55,255,THRESH_BINARY);
	 IplImage  iplImage = norm;
	 int nsize = norm.rows*norm.cols;
	 int *label = new int[norm.rows*norm.cols];
	 //int *lset = new int[norm.rows*norm.cols];
	 
	 int k = bwlabel(&iplImage, 8, label);
	 int *count = new int[k+1]();
	 
	 Mat result(norm.size(),CV_8U,Scalar(0));
	 cout<<k<<endl;
	 
	 for(int i = 0;i<nsize;i++){
		 if(label[i]){
		   int k1=label[i];
		   count[k1]++;
		 }
	 }

	 for(int i = 0;i<nsize;i++){
	    int nlab = label[i];
		
		if(count[nlab]>300){
			int m = i/result.cols;
		    int n = i%result.cols;
			result.at<uchar>(m,n) = 255;
	 }
	 }
	 delete label;
	 delete count;
	 Canny(this->img,this->contours,0.4*this->Get_HighThresh(),this->Get_HighThresh());
	 for(int i = 0;i<result.rows;i++){
		 for(int j = 0;j<result.cols;j++){
			 if(this->contours.at<uchar>(i,j) && (result.at<uchar>(i,j)== 0))this->contours.at<uchar>(i,j) = 0;
		 }
	 }
	 return this->contours;
	/* cv::findContours(norm,contours,CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	 int cArea = 100;
	 int ccon =contours.size();

	 for(int i = 0;i<ccon;i++){
		 if(contourArea(contours[i])>299)contours1.push_back(contours[i]);
		 
	 }
	
	 Mat result(norm.size(),CV_8U,Scalar(255));
	 drawContours(result,contours1,-1,Scalar(0),1);*/
	// namedWindow("sobel_image",1);
	// imshow("sobel_image",result);
	 
	 //waitKey();
	 
}

outline::~outline(){
	
	}
