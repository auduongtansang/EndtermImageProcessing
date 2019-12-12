#pragma once
#include <opencv2/opencv.hpp>
#include "Convolution.h"
#include "Blur.h"
#include <math.h>

using namespace std;
using namespace cv;

class CannyEdgeDetector
{
	//Ngưỡng dưới
	int _lowThreshold;
	//Ngưỡng trên
	int _highThreshold;

public:
	/*
	Hàm áp dụng thuật toán Canny để phát hiện biên cạnh
		srcImage: ảnh input
		dstImage: ảnh kết quả
	Hàm trả về
		1: nếu detect thành công
		0: nếu detect không thành công
	*/
	void Set(int Min, int Max);

	//Xét sự tồn tại của lân cận điểm biên có độ sáng > highThreshold
	bool Exist(Mat S, int x, int y);

	int Apply(const Mat& srcImage, Mat& dstImage);

	CannyEdgeDetector();
	~CannyEdgeDetector();
};
