#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<vector>

using namespace cv;
using namespace std;


void CreateHist(Mat src,int bins, float* range)
{
	////这里写成数组的形式，是因为calcHist的第一个图片输入参数可以是一个图片数组。
	int hist_size[] = { bins,bins,bins };
	float h_range[] = { 0.f, 360.f };
	float s_range[] = { 0.f, 1.f };
	float v_range[] = { 0.f, 1.f };
	//这里写成该形式也是一样
	const float* ranges[] = { h_range,s_range,v_range };
	MatND hist;
	int channels[] = { 0 ,0,0};
	calcHist()
	calcHist(srcs, 3/*源图像的个数*/, channels, Mat(), // do not use mask  
		hist, 1, hist_size, ranges,
		true, // the histogram is uniform  
		false);

	double max_val;
	minMaxLoc(hist, 0, &max_val, 0, 0);
	int scale = 100;
	int hist_height = 720;
	Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3);
	for (int i = 0; i<bins; i++)
	{
		float bin_val = hist.at<float>(i);
		int intensity = cvRound(bin_val*hist_height / max_val);  //要绘制的高度  
		rectangle(hist_img, Point(i*scale, hist_height - 1),
			Point((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	imshow("Source", src);
	imshow("Gray Histogram", hist_img);
}

int main()
{
	Mat src = imread("hand.jpg", IMREAD_UNCHANGED);
	//转换到HSV空间，默认是RGB空间
	cvtColor(src, src, CV_BGR2HSV);
	//将手部图片拆分成3个通道HSV，然后统计直方图，查看效果。
	//Hue, Saturation, Value色调、饱和度、亮度
	Mat h_src, s_src, v_src;
	vector<Mat> channels;
	split(src, channels);
	h_src = channels[0];
	s_src = channels[1];
	v_src = channels[2];
	imshow("h", h_src);
	imshow("s", s_src);
	imshow("v", v_src);

	//CreateHist();

	waitKey();
	return 0;
}