#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<vector>
#include<iostream>

using namespace cv;
using namespace std;


Mat CreateHist(Mat src,int bins, float* range)
{
	//这里写成数组的形式，是因为calcHist的第一个图片输入参数可以是一个图片数组。
	int hist_size[] = { bins };

	//这里写成该形式也是一样
	const float* ranges[] = { range };
	MatND hist;
	int channels[] = { 0};
	calcHist(&src, 1/*源图像的个数*/, channels, Mat(), // do not use mask  
		hist, 1/*直方图的维数*/, hist_size, ranges,
		true, // the histogram is uniform  
		false);

	double max_val;
	minMaxLoc(hist, 0, &max_val, 0, 0);
	int scale = 50;
	int hist_height = 480;
	Mat hist_img = Mat::zeros(hist_height, bins*scale, CV_8UC3);
	for (int i = 0; i<bins; i++)
	{
		float bin_val = hist.at<float>(i);
		int intensity = cvRound(bin_val*hist_height / max_val);  //要绘制的高度  
		rectangle(hist_img, Point(i*scale, hist_height - 1),
			Point((i + 1)*scale - 1, hist_height - intensity),
			CV_RGB(255, 255, 255));
	}
	return hist_img;
	//imshow("Source", src);
	//imshow("Gray Histogram", hist_img);
}

vector<Mat> GetEachChannelHist(Mat src)
{
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

	int bins = 16;
	//记住在OpenCV中，这三个通道的取值范围。
	float h_range[] = { 0,180 };
	float s_range[] = { 0,255 };
	float v_range[] = { 0,255 };
	Mat h_hist = CreateHist(h_src, bins, h_range);
	Mat s_hist = CreateHist(s_src, bins, s_range);
	Mat v_hist = CreateHist(v_src, bins, v_range);

	//定义数组返回，需要自己new，不然当你返回的时候地址是没有问题，但是指向的值就变了。
	vector<Mat> channelHists;
	channelHists.push_back(h_hist);
	channelHists.push_back(s_hist);
	channelHists.push_back(v_hist);
	return channelHists;
}

int main()
{
	Mat src1 = imread("hand.jpg", IMREAD_UNCHANGED);
	//转换到HSV空间，默认是RGB空间
	cvtColor(src1, src1, CV_BGR2HSV);
	vector<Mat> channelHists1= GetEachChannelHist(src1);

	Mat src2 = imread("hand02.jpg", IMREAD_UNCHANGED);
	//转换到HSV空间，默认是RGB空间
	cvtColor(src2, src2, CV_BGR2HSV);
	vector<Mat> channelHists2 = GetEachChannelHist(src2);

	Mat aa= channelHists2[0];
	imshow("test",aa);
	double CHISQR1 = compareHist(channelHists1[0], channelHists2[0], CV_COMP_CHISQR);
	cout << "CHISQR:" << CHISQR1 << endl;

	for (int i = 0; i < 3; i++)
	{
		double CHISQR = compareHist(channelHists1[i], channelHists2[i], CV_COMP_CHISQR);
		cout << "CHISQR:" << CHISQR << endl;
	}

	//imshow("h_hist",h_hist);
	//imshow("s_hist", s_hist);
	//imshow("v_hist", v_hist);
	//CreateHist();

	waitKey();
	return 0;
}