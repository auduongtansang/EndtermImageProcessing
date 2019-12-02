#pragma once
#include <opencv2/opencv.hpp>
#include <complex>
#include <math.h>
#include "FourierTransform.h"

using namespace std;
using namespace cv;

class ComplexImage
{
	//Kích thước của ảnh
	int rows, cols;

	//Trạng thái hiện tại của ảnh (đang ở dạng không gian hay dạng tần số)
	bool isFourierTransformed;

public:
	//Ma trận ảnh phức
	complex<double>* data;

	//Kích thước data
	int dataRows, dataCols;

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