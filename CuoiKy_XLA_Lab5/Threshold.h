#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>


using namespace cv;

//phân ngưỡng tĩnh
class StaticThreshold
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;
public:
	/*
	Hàm áp dụng phân ngưỡng tĩnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả	
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);


	StaticThreshold();
	StaticThreshold(int low, int high);
	StaticThreshold(const StaticThreshold &D);
	~StaticThreshold();
};


//phân ngưỡng cục bộ dựa vào trung bình
class AverageLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ theo trung bình
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
		1: nếu phân ngưỡng thành công
		0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	AverageLocalThreshold();
	AverageLocalThreshold(int d_C);
	AverageLocalThreshold(const AverageLocalThreshold &D);
	~AverageLocalThreshold();
};

//phân ngưỡng cục bộ dựa vào trung vị
class MedianLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ dựa vào trung vị
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	MedianLocalThreshold();
	MedianLocalThreshold(int d_C);
	MedianLocalThreshold(const MedianLocalThreshold &D);
	~MedianLocalThreshold();
};

//phân ngưỡng cục bộ dựa vào thuật toán Sauvola
class SauvolaLocalThreshold
{
	//hệ số r
	int _r;
	//hệ số k
	float _k;
public:
	/*
	Hàm áp dụng thuật toán Sauvola để phân ngưỡng
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	SauvolaLocalThreshold();
	SauvolaLocalThreshold(int d_r, float d_k);
	SauvolaLocalThreshold(const SauvolaLocalThreshold &D);
	~SauvolaLocalThreshold();
};

