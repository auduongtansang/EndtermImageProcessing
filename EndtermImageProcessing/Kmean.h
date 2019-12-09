#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace cv;
using namespace std;

class Kmean
{
	//số cụm K
	int _numClusters;

public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
		srcImage: ảnh input
		dstImage: ảnh kết quả
	Hàm trả về
		1: nếu phân đoạn thành công
		0: nếu phân đoạn không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);

	Kmean();
	Kmean(int d);
	Kmean(const Kmean &D);
	~Kmean();
};

float euclid_distance(float ax, float ay, float az, float bx, float by, float bz);