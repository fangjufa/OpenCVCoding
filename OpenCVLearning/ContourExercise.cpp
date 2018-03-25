#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\core.hpp>
#include<opencv2\opencv.hpp>
#include<vector>
#include"Helper.h"
//#include<string>

using namespace std;
using namespace cv;

int main()
{
	//Mat src = imread("car.jpg",IMREAD_GRAYSCALE);

	//Canny(src, src, 60, 255);

	//Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC1);

	////src = src > 1;
	//imshow("Source", src);

	//vector<vector<Point> > contours;
	//vector<Vec4i> hierarchy;
	//findContours(src, contours, hierarchy,
	//	CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//// iterate through all the top-level contours,
	//// draw each connected component with its own random color
	//int idx = 0;
	//for (; idx >= 0; idx = hierarchy[idx][0])
	//{
	//	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	//	drawContours(dst, contours, idx, color, CV_FILLED, 8, hierarchy);
	//}

	//imshow("Components", dst);
	//waitKey(0);
	
	Mat src = imread("circle.jpg", IMREAD_GRAYSCALE);
	Canny(src, src, 60, 128);
	imshow("Canny", src);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findAndDrawContours(src, contours, hierarchy,CV_RETR_CCOMP);

	for (int i = 0; i < contours.size(); i++)
	{
		double length = arcLength(contours[i], true);
		cout <<i<< "ÂÖÀª³¤¶È" << length << endl;
	}

	//approxPolyDP()


	//findAndDrawContours()



	waitKey(0);

	return 0;
}