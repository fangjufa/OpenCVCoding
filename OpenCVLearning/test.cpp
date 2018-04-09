#include<opencv2\opencv.hpp>
#include<stdlib.h>
#include"Helper.h"

using namespace std;
using namespace cv;

int test_main()
//int main()
{
	///测试梯度直方图
	//Mat img = imread("hand02.jpg",IMREAD_COLOR);
	//imshow("show", img);
	//Mat dst;

	////CreateGradientImage(img);
	//gradientGray(img, dst);
	//imshow("gradient", dst);

	///test accumlate function
	//Mat red = imread("red.jpg", IMREAD_UNCHANGED);
	//Mat cyan = imread("cyan.jpg", IMREAD_UNCHANGED);
	//Mat dst = Mat::zeros(red.rows, red.cols, CV_64FC3);

	//imshow("first", dst);
	//accumulate(red, dst);
	//imshow("second", dst);
	//accumulate(cyan, dst);
	//imshow("last", dst);
	////inrange

	//vector<float> aa = {1.0f,5.3f,4.6f};
	//vector<float> bb = { 0.5f,8.0f,6.f };
	//vector<float> cc = {2.f,10.f,7.f};
	//vector<float> dd(aa.size());

	//inRange(aa, bb, cc, dd);

	//for (int i = 0; i < 3; i++)
	//{
	//	printf("item:%f.\n", dd[i]);
	//}

	///测试convertTo函数
	Mat src = imread("car.jpg", IMREAD_UNCHANGED);
	printf("before type:%d.", src.type());
	//Mat srcF = Mat::zeros(src.rows, src.cols, CV_32FC3);
	src.convertTo(src, CV_32FC3, 1.0f/255.0);
	printf("before type:%d.", src.type());
	imshow("src", src);

	int key = 0;
	while (key != 27)
	{
		key = waitKey(0);
	}
	return 0;
}