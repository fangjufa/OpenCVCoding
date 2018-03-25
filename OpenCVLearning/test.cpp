#include<opencv2\opencv.hpp>
#include<stdlib.h>
#include"Helper.h"

using namespace std;
using namespace cv;

int test_main()
//int main()
{
	Mat img = imread("hand02.jpg",IMREAD_COLOR);
	imshow("show", img);
	Mat dst;

	//CreateGradientImage(img);
	gradientGray(img, dst);
	imshow("gradient", dst);

	int key = 0;
	while (key != 27)
	{
		key = waitKey(0);
	}
	return 0;
}