#include<opencv2\highgui.hpp>
#include<opencv2\core.hpp>
#include<opencv2\imgproc.hpp>

using namespace cv;

int t_main()
{
	RNG rng;
	float arr[1000];

	/*   随机生成一个数组    */
	for (int i = 0; i < 1000; i++)
	{
		arr[i] = rng.uniform(0.f, 1.f);
	}

	//绘制直方图
	//calcHist()
	//直立方块的个数。
	int bins = 10;
	////这里写成数组的形式，是因为calcHist的第一个图片输入参数可以是一个图片数组。
	int hist_size[] = { bins };
	float range[] = { 0.f, 1.f };
	//这里写成该形式也是一样
	const float* ranges[] = { range };
	MatND hist;
	int channels[] = { 0 };

	//Mat src = Mat(100, 10, CV_32FC1,arr);
	Mat src = Mat(Size(100,10),CV_32FC1,arr);

	
	calcHist(&src, 1/*源图像的个数*/, channels, Mat(), // do not use mask  
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
	waitKey();

	return 0;
}