#pragma once
#include <opencv2/opencv.hpp>
#include "FourierTransform.h"
#include <complex>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

class ComplexImage
{
	//Ma trận ảnh phức
	vector<vector<complex<double>>> data;

	//Kích thước của ảnh
	int rows, cols;

	//Trạng thái hiện tại của ảnh (đang ở dạng không gian hay dạng tần số)
	bool isFourierTransformed;

public:
	ComplexImage();
	~ComplexImage();

	//Tạo ComplexImage từ cấu trúc Mat của OpenCV
	void FromOpenCVMat(Mat image);

	//Tạo OpenCV Mat từ ComplexImage
	Mat ToOpenCVMat();

	//Biến đổi Fourier thuận
	void ForwardFourierTransform();

	//Biến đổi Fourier nghịch
	void BackwardFourierTransform();
};