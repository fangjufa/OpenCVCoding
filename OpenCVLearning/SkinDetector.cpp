#include"Helper.h"
#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
#include<iostream>
#include<vector>
#include<string>


using namespace std;
using namespace cv;

//Scalar hand;
Mat src, hsv, mask,hand;
int lo = 20, up = 20;

void Hist_and_Backproj();

void pickPoint(int event, int x, int y, int, void*)
{
	if (event != CV_EVENT_LBUTTONDOWN)
	{
		return;
	}
	Point seed = Point(x, y);

	int newMaskVal = 255;
	Scalar newVal = Scalar(120, 120, 120);

	int connectivity = 8;
	int flags = connectivity + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

	Mat mask2 = Mat::zeros(src.rows + 2, src.cols + 2, CV_8UC1);
	floodFill(src, mask2, seed, newVal, 0, Scalar(lo, lo, lo), Scalar(up, up, up), flags);
	mask = mask2(Range(1, mask2.rows - 1), Range(1, mask2.cols - 1));

	imshow("Mask", mask);

	Hist_and_Backproj();
}

void Hist_and_Backproj()
{
	MatND hist;
	hand = Scalar::all(0);
	int h_bins = 30; int s_bins = 32;
	int histSize[] = { h_bins, s_bins };

	float h_range[] = { 0, 179 };
	float s_range[] = { 0, 255 };
	const float* ranges[] = { h_range, s_range };

	int channels[] = { 0, 1 };

	/// Get the Histogram and normalize it  
	calcHist(&hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false);

	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	/// Get Backprojection  
	Mat backproj;
	calcBackProject(&hsv, 1, channels, hist, backproj, ranges, 1, true);

	/// Draw the backproj  
	imshow("BackProj", backproj);
	/*src.copyTo(hand, backproj);
	imshow("hand", hand);*/
}

int skin_main()
{
	//VideoCapture cap(0); // open the default camera
	//if (!cap.isOpened())  // check if we succeeded
	//	return -1;
	//向左移8位
	//std::cout << (255 << 8) << endl;
	src = imread("hand02.jpg", IMREAD_UNCHANGED);
	cvtColor(src, hsv, CV_BGR2HSV);
	string window_image = "aaa";

	int val = 10;
	
	imshow(window_image, src);
	//int lo = 0,up = 0;
	//createTrackbar("* Hue  bins: ", window_image, &bins, 180, hue_and_Backproj);
	//createTrackbar("test_trackbar", window_image, &val, 255);
	createTrackbar("Low thresh", window_image, &lo, 255, 0);
	createTrackbar("High thresh", window_image, &up, 255, 0);
	/// Set a Mouse Callback  
	//hue_and_Backproj(0, 0);
	setMouseCallback(window_image, pickPoint, 0);

	//for (;;)
	//{
	//	Mat frame;
	//	cap >> frame; // get a new frame from camera

	//	imshow("edges", frame);
	//	/*vector<Mat> rgbMats;
	//	split(frame, rgbMats);*/

	//	cvtColor(frame, frame, CV_RGB2HSV);
	//	Hist_and_Backproj();



	//	//float range[] = {0.f,255.f};
	//	//for (int i = 0; i < 3; i++)
	//	//{
	//	//	Mat channel = CreateHist(rgbMats[i], 32, range);
	//	//	string windName = to_string(i);
	//	//	imshow(windName, channel);
	//	//}

	//	if (waitKey(50) >= 0) break;
	//}
	waitKey(0);
	return 0;
}