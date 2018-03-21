#pragma once
#ifndef _HELPER_
#define _HELPER_


#include<opencv2\imgproc.hpp>
#include<opencv2\core.hpp>
using namespace cv;

Mat CreateHist(Mat src, int bins, float* range);

Mat CreateGradientImage(Mat img);

void gradientGray(Mat &src, Mat &dst);

#endif