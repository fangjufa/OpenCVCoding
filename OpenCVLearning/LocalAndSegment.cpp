#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
//#include<opencv2\video.hpp>
using namespace cv;

int main()
{
	//����֡��ķ�ʽ��ȡ���������õ�ǰ֡ͼ�������ؼ�ȥ��һ֡ͼ��
	VideoCapture vc;

	if (!vc.open(0))
	{
		printf("������ͷʧ�ܣ�");
		return -1;
	}

	Mat currentFrame,previousFrame,foreground,temp;

	int count = 0;
	int key=0;
	while (key != 27)
	{
		vc.read(temp);
		cvtColor(temp, temp, CV_RGB2GRAY);
		if (count == 0)
		{
			currentFrame = temp;
			previousFrame = temp;
		}
		else
		{
			previousFrame = currentFrame;
			currentFrame = temp;
			imshow("current frame pic", currentFrame);
			
			absdiff(previousFrame, currentFrame, foreground);
			imshow("fore ground", foreground);

			threshold(foreground, foreground, 15, 255, CV_THRESH_BINARY);
			imshow("after threshold", foreground);
		}
		
		count++;
		//30ms waiting,read next frame.
		key = waitKey(30);
	}

	return 0;
}