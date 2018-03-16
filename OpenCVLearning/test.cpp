#include<opencv2\opencv.hpp>
#include<stdlib.h>

using namespace std;
using namespace cv;

int test_main()
{
	Mat img = imread("aa.png",IMREAD_COLOR);
	imshow("show", img);

	int key = 0;
	while (key != 27)
	{
		key = waitKey(0);
	}
	return 0;
}