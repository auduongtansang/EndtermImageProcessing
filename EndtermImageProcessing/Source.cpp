#include <opencv2/opencv.hpp>
#include "ComplexImage.h"
#include "FrequencyFilter.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	if (argc < 4)
		return -1;

	Mat img, dst;

	if (strcmp(argv[1], "--fft") == 0)
	{
		//Load ảnh
		img = imread(argv[2], IMREAD_GRAYSCALE);
		imshow("Original", img);

		//Chuyển sang dạng ảnh phức
		ComplexImage cImg;
		cImg.FromOpenCVMat(img);

		//Biến đổi Fourier thuận
		cImg.ForwardFourierTransform();
		dst = cImg.ToOpenCVMat();

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Frequency", dst);
	}
	else if (strcmp(argv[1], "--ifft") == 0)
	{
		//Load ảnh
		img = imread(argv[2], IMREAD_GRAYSCALE);
		imshow("Original", img);

		//Chuyển sang dạng ảnh phức
		ComplexImage cImg;
		cImg.FromOpenCVMat(img);

		//Biến đổi Fourier thuận
		cImg.ForwardFourierTransform();
		imshow("Frequency", cImg.ToOpenCVMat());

		//Biến đổi Fourier nghịch
		cImg.BackwardFourierTransform();
		dst = cImg.ToOpenCVMat();

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Inversion", dst);
	}
	else if (strcmp(argv[1], "--freq") == 0)
	{
		if (argc < 8)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_GRAYSCALE);
		imshow("Original", img);

		//Chuyển sang dạng ảnh phức
		ComplexImage cImg;
		cImg.FromOpenCVMat(img);

		//Biến đổi Fourier thuận
		cImg.ForwardFourierTransform();
		imshow("Frequency", cImg.ToOpenCVMat());

		int minX = stoi(argv[4]), maxX = stoi(argv[5]), minY = stoi(argv[6]), maxY = stoi(argv[7]);
		FrequencyFilter filter(minX, maxX, minY, maxY);
		filter.Apply(cImg);

		//Hiển thị ảnh tần số đã lọc
		imshow("Filtered (frequency)", cImg.ToOpenCVMat());

		//Biến đổi Fourier nghịch
		cImg.BackwardFourierTransform();
		dst = cImg.ToOpenCVMat();

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Filtered (inversion)", dst);
	}

	////Load ảnh test
	//Mat img = imread("periodicLow.png", IMREAD_GRAYSCALE);
	//imshow("Original", img);

	////Chuyển sang dạng ảnh phức
	//ComplexImage cImg;
	//cImg.FromOpenCVMat(img);

	////Biến đổi Fourier thuận
	//cImg.ForwardFourierTransform();
	//imshow("Frequency", cImg.ToOpenCVMat());

	////Lọc tần số
	//FrequencyFilter filter(200, 312, 390, 634);
	//filter.Apply(cImg);
	//imshow("Filtered", cImg.ToOpenCVMat());

	////Biến đổi Fourier nghịch
	//cImg.BackwardFourierTransform();
	//imshow("Inverse", cImg.ToOpenCVMat());

	waitKey(0);
	return 0;
}