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
	Mat img = imread("cameraman.bmp", IMREAD_GRAYSCALE);
	imshow("Original", img);

	//Chuyển sang dạng ảnh phức
	ComplexImage cImg;
	cImg.FromOpenCVMat(img);

	//Biến đổi Fourier thuận
	cImg.ForwardFourierTransform();

	//Chuyển sang dạng ảnh thực và hiển thị
	Mat fImg = cImg.ToOpenCVMat();
	imshow("Frequency", fImg);

	waitKey(0);
	return 0;
}