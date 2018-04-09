﻿#include<opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
//#include<opencv2\>

using namespace cv;

///定义codebook数据类型
#define CHANNELS 3
typedef struct ce
{
	uchar learnHigh[CHANNELS];
	uchar learnLow[CHANNELS];
	uchar max[CHANNELS];//这个最大最小值的作用还不太清楚。
	uchar min[CHANNELS];
	int t_last_update;
	int stale;
}code_element;

typedef struct cb
{
	code_element** ces;
	int numEntries;
	int t;
}code_book;

///逐像素更新codebook。
///其中p就是像素的指针，c是该像素上的codebook,cbBounds是一个偏移值。
void update_codebook(uchar* p, code_book& c, unsigned* cbBounds)
{
	uint low[CHANNELS], high[CHANNELS];
	for (int i = 0; i < CHANNELS; i++)
	{
		high[i] = *(p + i) + *(cbBounds + i);
		if (high[i] > 255)
			high[i] = 255;
		low[i] = *(p + i) - *(cbBounds + i);
		if (low[i] < 0)
			low[i] = 0;
	}

	int matchChannel = 0;
	int i = 0;
	for (; i < c.numEntries; i++)
	{
		matchChannel = 0;
		for (int n = 0; n < CHANNELS; n++)
		{
			//如果该像素的值在范围内，则匹配。
			if (c.ces[i]->learnHigh[n] > *(p + n) &&
				c.ces[i]->learnLow[n] < *(p + n))
			{
				matchChannel++;
			}
		}
		//如果该像素的所有通道都在范围内，则更新这个像素的codebook。
		if (matchChannel >= CHANNELS)
		{
			c.ces[i]->t_last_update = c.t;
			for (int n = 0; n < CHANNELS; n++)
			{
				if (c.ces[i]->max[n] < *(p + n))
					c.ces[i]->max[n] = *(p + n);
				if (c.ces[i]->min[n] > *(p + n))
					c.ces[i]->min[n] = *(p + n);
			}
			break;
		}//end if matchChannel >= CHANNELS
	}//end for numEntries
	//更新一下有多久没有访问该条目。
	for (int s = 0; s < c.numEntries; s++)
	{
		int negRun = c.t - c.ces[s]->t_last_update;
		if (c.ces[s]->stale < negRun)
		{
			c.ces[s]->stale = negRun;
		}
	}
	//如果遍历完了所有的entries,还是没有找到匹配的codeelement,那么就重新生成一个code_element.
	if (i >= c.numEntries)
	{
		code_element** foo = new code_element*[c.numEntries + 1];
		for (int ii = 0; ii < c.numEntries; ii++)
		{
			foo[ii] = c.ces[ii];
		}
		foo[c.numEntries] = new code_element;
		//删除原来的数组指针。
		if (c.numEntries) delete [] c.ces;
		c.ces = foo;
		for (int n = 0; n < CHANNELS; n++)
		{
			c.ces[c.numEntries]->learnHigh[n] = high[n];
			c.ces[c.numEntries]->learnLow[n] = low[n];
			c.ces[c.numEntries]->max[n] = *(p+n);
			c.ces[c.numEntries]->min[n] = *(p + n);
		}
		c.ces[c.numEntries]->stale = 0;
		c.ces[c.numEntries]->t_last_update = c.t;

		c.numEntries += 1;
	}
	///下面这一段的作用不太清楚
	for (int n = 0; n < CHANNELS; n++)
	{
		if (c.ces[i]->learnHigh[n] < high[n])
			c.ces[i]->learnHigh[n]++;
		if (c.ces[i]->learnLow[n] > low[n])
			c.ces[i]->learnLow[n]--;
	}
}

///清除不经常访问的条目，减少噪声带来的影响
void clearStaleEntries(code_book& c)
{
	//标记数组，标记为0，则是需要清除的，非0则不清除。
	int* keep = new int[c.numEntries];
	int keepCnt = 0;
	//清除的阈值，经验为其一半。
	int staleThresh = c.t >> 1;
	for (int i = 0; i < c.numEntries; i++)
	{
		if (c.ces[i]->stale > staleThresh)
			keep[i] = 0;
		else
		{
			keep[i] = 1;
			keepCnt++;
		}
	}

	//开始清除。
	c.t = 0;
	code_element** foo = new code_element*[keepCnt];
	int index = 0;
	for (int i = 0; i < c.numEntries; i++)
	{
		if (keep[i] != 0)
		{
			foo[index] = c.ces[i];
			foo[index]->t_last_update = 0;
			index++;
		}
	}
	delete[] keep;
	delete[] c.ces;
	c.ces = foo;
	c.numEntries = keepCnt;
}

///背景差分，给定像素，通过判断codebook，来判断是背景像素还是前景像素。
///返回值就是0代表背景，或者255代表前景。
uchar backGroundDiff(code_book& c,uchar* p,int* minMod,int* maxMod)
{
	int matchChannel;
	int i = 0;
	for (; i < c.numEntries; i++)
	{
		matchChannel = 0;
		for (int n = 0; n < CHANNELS; n++)
		{
			//这里的像素值比较又与前面不同，不知道为什么要这么做。
			if (c.ces[i]->min[n] - minMod[n] <= *(p + n) &&
				c.ces[i]->max[n] + maxMod[n] >= *(p + n))
				matchChannel++;
			else
				break;
		}
		if (matchChannel == CHANNELS)
			break;
	}

	//说明该像素没有落在一个条目上，则是前景目标
	if (i >= c.numEntries)
		return 255;
	return 0;
}

int main()
{
	
	return 0;
}