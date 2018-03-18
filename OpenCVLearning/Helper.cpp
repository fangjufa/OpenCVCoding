#include"Helper.h"

Mat CreateHist(Mat src, int bins, float* range)
{
	//这里写成数组的形式，是因为calcHist的第一个图片输入参数可以是一个图片数组。
	int hist_size[] = { bins };

	//这里写成该形式也是一样
	const float* ranges[] = { range };
	MatND hist;
	int channels[] = { 0 };
	calcHist(&src, 1/*源图像的个数*/, channels, Mat(), // do not use mask  
		hist, 1/*直方图的维数*/, hist_size, ranges,
		true, // the histogram is uniform  
		false);


	//double max_val;
	//minMaxLoc(hist, 0, &max_val, 0, 0);
	//int scale = 50;
	//int hist_height = 480;
	//Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3);
	//for (int i = 0; i<bins; i++)
	//{
	//	float bin_val = hist.at<float>(i);
	//	int intensity = cvRound(bin_val*hist_height / max_val);  //要绘制的高度  
	//	rectangle(hist_img, Point(i*scale, hist_height - 1),
	//		Point((i + 1)*scale - 1, hist_height - intensity),
	//		CV_RGB(255, 255, 255));
	//}
	//return hist_img;
	//imshow("Source", src);
	//imshow("Gray Histogram", hist_img);
	return hist;
}