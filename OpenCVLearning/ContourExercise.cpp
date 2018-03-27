#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\core.hpp>
#include<opencv2\opencv.hpp>
#include<vector>
#include"Helper.h"
//#include<string>

using namespace std;
using namespace cv;

int contour_main()
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
	
	Mat src = imread("hand.jpg", IMREAD_GRAYSCALE);
	Canny(src, src, 128, 200);
	imshow("Canny", src);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cout << "contours length:" << contours.size() << endl;
	cout << "hierarchy length:" << hierarchy.size() << endl;
	findAndDrawContours(src, contours, hierarchy,CV_RETR_CCOMP);
	cout << "contours length:" << contours.size() << endl;
	cout << "hierarchy length:" << hierarchy.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		double length = arcLength(contours[i], true);
		cout <<i<< "ÂÖÀª³¤¶È" << length << endl;
	}


	vector<vector<Point>> poly(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], poly[i], 0.1f, true);
	}
	RNG rng;
	Mat contourImg = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (int i = 0; i < poly.size(); i++)
	{
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(contourImg, contours, i, Scalar(128,0,0,255), 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(contourImg, poly, i, Scalar(0, 0, 255, 255), 1, 8, vector<Vec4i>(), 0, Point());
	}

	imshow("approxPolyDP", contourImg);
	//approxPolyDP()


	//findAndDrawContours()



	waitKey(0);

	return 0;
}