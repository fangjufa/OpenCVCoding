#pragma once
#ifndef _HELPER_
#define _HELPER_


#include<opencv2\imgproc.hpp>
using namespace cv;

Mat CreateHist(Mat src, int bins, float* range);

#endif