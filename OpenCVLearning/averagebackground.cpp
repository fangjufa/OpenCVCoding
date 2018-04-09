#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

//需要积累的帧数。
float Icount;
//定义各种矩阵变量
Mat IavgF, IdiffF, IpreF, IhiF, IlowF;
Mat Iscratch, Iscratch2;

//单通道的图像，由IhiF,IlowF等拆分而成

Mat Igray1, Igray2, Igray3;
Mat Ilow1, Ilow2, Ilow3;
Mat Ihi1, Ihi2, Ihi3;
vector<Mat> Igrays;
vector<Mat> Ilows;
vector<Mat> Ihis;
Mat maskT;
Mat maski;

void setHighThreshold(float scale);
void setLowThreshold(float scale);

///初始化上面定义的各种矩阵，src是摄像头捕捉到的一帧画面。
void InitialImgs(Mat src)
{
	Icount = 0.01f;
	Size s = Size(src.cols, src.rows);
	IavgF = Mat::zeros(s, CV_32FC3);
	IdiffF = Mat::zeros(s, CV_32FC3);
	IpreF = Mat::zeros(s, CV_32FC3);
	IhiF = Mat::zeros(s, CV_32FC3);
	IlowF = Mat::zeros(s, CV_32FC3);
	Iscratch = Mat::zeros(s, CV_32FC3);
	Iscratch2 = Mat::zeros(s, CV_32FC3);

	maskT = Mat::zeros(s, CV_8UC1);
	maski = Mat::zeros(s, CV_8UC1);

	Igrays = { Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) };
	Ilows = { Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) };
	Ihis = { Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) ,Mat::zeros(s, CV_32FC1) };;
	//Igray1 = Mat::zeros(s, CV_32FC1);
	//Igray2 = Mat::zeros(s, CV_32FC1);
	//Igray3 = Mat::zeros(s, CV_32FC1);
	//Ilow1 = Mat::zeros(s, CV_32FC1);
	//Ilow2 = Mat::zeros(s, CV_32FC1);
	//Ilow3 = Mat::zeros(s, CV_32FC1);
	//Ihi1 = Mat::zeros(s, CV_32FC1);
	//Ihi2 = Mat::zeros(s, CV_32FC1);
	//Ihi3 = Mat::zeros(s, CV_32FC1);

	
}

///累积图像,参数是捕获的摄像头的一帧图像
void accumulateBackground(Mat src)
{
	static int first = 1;
	src.convertTo(Iscratch, CV_32FC3, 1.0f, 0.0f);//convert to float

	if (!first)
	{
		//求积
		accumulate(Iscratch, IavgF);
		//求绝对值差
		absdiff(Iscratch, IpreF, Iscratch2);
		accumulate(Iscratch2, IdiffF);
		Icount += 1.0f;
	}
	first = 0;
	Iscratch.copyTo(IpreF);
}

///创建模型
void createModelsFromStats()
{
	convertScaleAbs(IavgF, IavgF, (double)(1.0 / Icount));

	convertScaleAbs(IdiffF, IdiffF, (double)(1.0 / Icount));

	//这里加1是因为怕到时候除的时候会出现除数为0的情况。
	add(IdiffF, Mat::ones(Size(IdiffF.cols,IdiffF.rows), IdiffF.type()), IdiffF);
	setHighThreshold(7.0);
	setLowThreshold(6.0);
}

void setHighThreshold(float scale)
{
	convertScaleAbs(IdiffF, Iscratch, scale);
	add(Iscratch, IavgF, IhiF);
	split(IhiF,Ihis);
}

void setLowThreshold(float scale)
{
	convertScaleAbs(IdiffF, Iscratch, scale);
	subtract(IavgF, Iscratch, IlowF);
	//add(Iscratch, IavgF, IhiF);
	split(IlowF, Ilows);
}

void backgroundDiff(Mat src,Mat iMask)
{
	src.convertTo(Iscratch, 1.0f, 0);
	split(Iscratch, Igrays);

	inRange(Igrays[0], Ilows[0], Ihis[0], iMask);

	inRange(Igrays[1], Ilows[1], Ihis[1], maskT);
	bitwise_or(iMask, maskT, iMask);
	inRange(Igrays[2], Ilows[2], Ihis[2], maskT);

	bitwise_or(iMask, maskT, iMask);

	subtract(iMask, 255, iMask);
}

int main()
{
	VideoCapture cap;
	if (!cap.open(0))
		return -1;

	cvNamedWindow("raw");
	cvNamedWindow("avg");

	Mat frameImg;
	cap.read(frameImg);
	//IplImage* rawImage = cvQueryFrame(capture);
	//这个函数仅仅是函数cvGrabFrame和函数cvRetrieveFrame在一起调用的组合  
	imshow("raw", frameImg);

	InitialImgs(frameImg);

	for (int i = 0;; i++)
	{
		if (i <= 30)
		{
			accumulateBackground(frameImg);
			//前30帧用于累积计算背景图像  
			if (i == 30)
				//将前30真转换成一个背景统计模型 
				createModelsFromStats();
				//createModelsfromStats();
		}
		else
			//建立好背景模型后调用此函数进行图像分割  
			backgroundDiff(frameImg, maski);

		imshow("avg", maski);
		//播放分割后的目标图像结果  

		if (cvWaitKey(33) == 27)
			//每33ms 播放一帧  
			break;

		if (!cap.read(frameImg))
			break;
		imshow("raw", frameImg);
		//显示原图像  

		if (i == 56 || i == 63)
			//56帧和63帧时暂停  
			cvWaitKey();
	}

	//DeallocateImages();
	return 0;
}