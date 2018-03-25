#pragma once
#ifndef _HELPER_
#define _HELPER_


#include<opencv2\imgproc.hpp>
#include<opencv2\core.hpp>
#include<vector>
using namespace std;
using namespace cv;

Mat CreateHist(Mat src, int bins, float* range);

Mat CreateGradientImage(Mat img);

void gradientGray(Mat &src, Mat &dst);

void findAndDrawContours(Mat image, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy,
	int mode = CV_RETR_EXTERNAL, int method = CV_CHAIN_APPROX_SIMPLE, Point offset = Point());

#endif