#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
using namespace cv;

int func_main()
{
	Mat src = imread("hand02.jpg");
	Mat erodeImg;
	erode(src, erodeImg, Mat());

	imshow("src", src);
	imshow("erode", erodeImg);
	waitKey(0);
	return 0;
}