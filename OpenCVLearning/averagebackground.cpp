#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>

using namespace cv;

//定义各种矩阵变量
Mat IavgF, IdiffF, IpreF, IhiF, IlowF;
Mat Iscratch, Iscratch2;

//单通道的图像，由IhiF,IlowF等拆分而成
Mat Igray1, Igray2, Igray3;
Mat Ilow1, Ilow2, Ilow3;
Mat Ihi1, Ihi2, Ihi3;
Mat maskT;
//需要积累的帧数。
float count;

///初始化上面定义的各种矩阵，src是摄像头捕捉到的一帧画面。
void InitialImgs(Mat src)
{
	Size s = Size(src.cols, src.rows);
	IavgF = Mat::zeros(s, CV_32FC3);
	IdiffF = Mat::zeros(s, CV_32FC3);
	IpreF = Mat::zeros(s, CV_32FC3);
	IhiF = Mat::zeros(s, CV_32FC3);
	IlowF = Mat::zeros(s, CV_32FC3);
	Iscratch = Mat::zeros(s, CV_32FC3);
	Iscratch2 = Mat::zeros(s, CV_32FC3);

	maskT = Mat::zeros(s, CV_8UC1);

	Igray1 = Mat::zeros(s, CV_32FC1);
	Igray2 = Mat::zeros(s, CV_32FC1);
	Igray3 = Mat::zeros(s, CV_32FC1);
	Ilow1 = Mat::zeros(s, CV_32FC1);
	Ilow2 = Mat::zeros(s, CV_32FC1);
	Ilow3 = Mat::zeros(s, CV_32FC1);
	Ihi1 = Mat::zeros(s, CV_32FC1);
	Ihi2 = Mat::zeros(s, CV_32FC1);
	Ihi3 = Mat::zeros(s, CV_32FC1);

	count = 0.01f;
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
		count += 1.0f;
	}
	first = 0;
	Iscratch.copyTo(IpreF);
}

///创建模型
void createModelsFromStats()
{
	convertScaleAbs(IavgF, IavgF, (double)(1.0 / count));

	convertScaleAbs(IdiffF, IdiffF, (double)(1.0 / count));

	//这里加1是因为怕到时候除的时候会出现除数为0的情况。
	add(IdiffF, Mat::ones(Size(IdiffF.cols,IdiffF.rows), IdiffF.type()), IdiffF);
	setHighThreshold(7.0);
	setLowThreshold(6.0);
}

void setHighThreshold(float scale)
{

}

void setLowThreshold(float scale)
{

}


int a_main()
{
	VideoCapture cap;
	if (!cap.open(0))
		return -1;



	return 0;
}