#include"Helper.h"
#include<vector>
#include<opencv2\highgui.hpp>

using namespace std;

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

///创建梯度图像
Mat CreateGradientImage(Mat src)
{
	
	if(src.channels() != 1)
		cvtColor(src, src, CV_BGR2GRAY);

	//梯度矩阵，存放的是该像素点的梯度方向，大小是0～pi
	Mat dst = Mat(src.rows,src.cols,CV_32FC2);
	Mat xGradient(Size(src.cols,src.rows),src.type()),yGradient(Size(src.cols, src.rows), src.type());
	int xArr[] = {-1,0,1};
	int yArr[] = { 1,0,-1 };
	//Vec3f xArr = { -1.f,0.f,1.f };
	//Vec3f yArr = { 1.f,0.f,-1.f };
	Mat xKernel(1, 3, CV_32FC1, xArr);// = Mat(1, 3, CV_32FC1, xArr);
	Mat yKernel(3, 1, CV_32FC1, yArr);// = Mat(3, 1, CV_32FC1, yArr);
	filter2D(src, xGradient, -1, xKernel);
	filter2D(src, yGradient, -1, yKernel);

	for (int i = 0; i < src.cols; i++)
	{
		for (int j = 0; j < src.rows; j++)
		{
			if (j == 300)
				int aa = j;
			int* x= xGradient.ptr<int>(i, j);
			int* y = yGradient.ptr<int>(i, j);
			/*float x = xGradient.at<int>(i, j);
			int y = yGradient.at<int>(i, j);*/
			dst.at<Vec2f>(i, j)[0] = sqrt((*x)*(*x) + (*y)*(*y));
			dst.at<Vec2f>(i, j)[1] = cvFastArctan((*y), (*x));
			//Vec2f pixel;
			////pixel = dst.at<vector<float>>(i, j);// [0] = 
			//pixel..push_back(sqrt(x*x + y*y));
			//pixel.push_back(cvFastArctan(y, x));
			//dst.at<vector<float>>(i, j) = pixel;
		}
	}

	imshow("dst", dst);
	return dst;
}


///该得出的结果貌似不太能用。
void gradientGray(Mat &src, Mat &dst)
{
	const int H = src.rows, W = src.cols;
	Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);
	//因为计算出的梯度值可能有正有负，且值也可能会很大，故数据类型为整形  

	// 求水平方向梯度，处理左右边缘像素  
	for (int y = 0; y < H; y++) {
		Ix.at<int>(y, 0) = abs(src.at<char>(y, 1) - src.at<char>(y, 0)) * 2;
		for (int x = 1; x < W - 1; x++)
			Ix.at<int>(y, x) = abs(src.at<char>(y, x + 1) - src.at<char>(y, x - 1));
		Ix.at<int>(y, W - 1) = abs(src.at<char>(y, W - 1) - src.at<char>(y, W - 2)) * 2;
	}
	// 求垂直方向梯度，处理左右边缘像素  
	for (int x = 0; x < W; x++) {
		Iy.at<int>(0, x) = abs(src.at<char>(1, x) - src.at<char>(0, x)) * 2;
		for (int y = 1; y < H - 1; y++)
			Iy.at<int>(y, x) = abs(src.at<char>(y + 1, x) - src.at<char>(y - 1, x));
		Iy.at<int>(H - 1, x) = abs(src.at<char>(H - 1, x) - src.at<char>(H - 2, x)) * 2;
	}
	//for (int j = 0; j < H; j++)
	//	for (int k = 0; k < W; k++)
	//	{
	//		dst.at<char>(j, k) = min(Ix.at<int>(j,k) + Iy.at<int>(j, k), 255);
	//	}
	convertScaleAbs(min(Ix + Iy, 255), dst); //这句话和上面的for循环是同样的功能  
}