#include<iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class outline{
private:
	Mat img;
	int HighThresh;
	Mat contours;
	
public:
	outline();
	void Get_img(Mat &img);
	int Get_HighThresh();
	//void nfilter(Mat &img);
	Mat nfilter();
	
	
	
	~outline();
};