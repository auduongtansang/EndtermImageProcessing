#include <iostream>
#include <complex>
#include <vector>
#include "ComplexImage.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	//Load ảnh test
	Mat img = imread("cameraman.png", IMREAD_GRAYSCALE);
	imshow("Original", img);

	//Chuyển sang dạng ảnh phức
	ComplexImage cImg;
	cImg.FromOpenCVMat(img);

	//Biến đổi Fourier thuận
	cImg.ForwardFourierTransform();
	imshow("Frequency", cImg.ToOpenCVMat());

	//Biến đổi Fourier nghịch
	cImg.BackwardFourierTransform();
	imshow("Inverse", cImg.ToOpenCVMat());

	waitKey(0);
	return 0;
}