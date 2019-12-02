#include <opencv2/opencv.hpp>
#include "ComplexImage.h"
#include "FrequencyFilter.h"

using namespace std;
using namespace cv;

int main()
{
	//Load ảnh test
	Mat img = imread("periodicLow.png", IMREAD_GRAYSCALE);
	imshow("Original", img);

	//Chuyển sang dạng ảnh phức
	ComplexImage cImg;
	cImg.FromOpenCVMat(img);

	//Biến đổi Fourier thuận
	cImg.ForwardFourierTransform();
	imshow("Frequency", cImg.ToOpenCVMat());

	//Lọc tần số
	FrequencyFilter filter(200, 312, 390, 634);
	filter.Apply(cImg);
	imshow("Filtered", cImg.ToOpenCVMat());

	//Biến đổi Fourier nghịch
	cImg.BackwardFourierTransform();
	imshow("Inverse", cImg.ToOpenCVMat());

	waitKey(0);
	return 0;
}