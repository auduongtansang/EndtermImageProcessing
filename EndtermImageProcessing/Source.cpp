#include <opencv2/opencv.hpp>
#include "Kmean.h"
#include "Threshold.h"
#include "ComplexImage.h"
#include "FrequencyFilter.h"
#include "CannyEdgeDetector.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	if (argc < 4)
		return -1;

	Mat img, dst;

	//Biến đổi Fourier thuận
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
	
	//Biến đổi Fourier nghịch
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

	//Lọc tần số
	else if (strcmp(argv[1], "--freq") == 0)
	{
		//periodicLow ====> filter(200, 312, 390, 634);

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

	//Phân ngưỡng tĩnh
	else if (strcmp(argv[1], "--static") == 0)
	{
		if (argc < 6)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_COLOR);
		imshow("Original", img);

		//Phân ngưỡng
		int low = stoi(argv[4]), high = stoi(argv[5]);
		StaticThreshold thresholder(low, high);
		thresholder.Apply(img, dst);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Static thresholded", dst);
	}

	//Phân ngưỡng động theo trung bình
	else if (strcmp(argv[1], "--mean") == 0)
	{
		if (argc < 7)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_COLOR);
		imshow("Original", img);

		//Phân ngưỡng
		Size wSize(stoi(argv[4]), stoi(argv[5]));
		int c = stoi(argv[6]);
		AverageLocalThreshold thresholder(c);
		thresholder.Apply(img, dst, wSize);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Average local thresholded", dst);
	}

	//Phân ngưỡng động theo trung vị
	else if (strcmp(argv[1], "--median") == 0)
	{
		if (argc < 7)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_COLOR);
		imshow("Original", img);

		//Phân ngưỡng
		Size wSize(stoi(argv[4]), stoi(argv[5]));
		int c = stoi(argv[6]);
		MedianLocalThreshold thresholder(c);
		thresholder.Apply(img, dst, wSize);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Median local thresholded", dst);
	}

	//Phân ngưỡng động Sauvola
	else if (strcmp(argv[1], "--sauvola") == 0)
	{
		if (argc < 8)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_COLOR);
		imshow("Original", img);

		//Phân ngưỡng
		Size wSize(stoi(argv[4]), stoi(argv[5]));
		int r = stoi(argv[6]);
		float k = stof(argv[7]);
		SauvolaLocalThreshold thresholder(r, k);
		thresholder.Apply(img, dst, wSize);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Sauvola local thresholded", dst);
	}

	//Phân cụm theo K-means
	else if (strcmp(argv[1], "--kmeans") == 0)
	{
		if (argc < 5)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_COLOR);
		imshow("Original", img);

		//Phân cụm
		int k = stoi(argv[4]);
		Kmean clusterer(k);
		clusterer.Apply(img, dst);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("K-means clustered", dst);
	}

	//Lọc biên cạnh Canny
	else if (strcmp(argv[1], "--canny") == 0)
	{
		if (argc < 6)
			return -1;

		//Load ảnh
		img = imread(argv[2], IMREAD_GRAYSCALE);
		imshow("Original", img);

		//Lọc biên
		int low = stoi(argv[4]), high = stoi(argv[5]);
		CannyEdgeDetector detector;
		detector.Set(low, high);
		detector.Apply(img, dst);

		//Ghi và hiển thị ảnh
		imwrite(argv[3], dst);
		imshow("Canny edge detected", dst);
	}

	waitKey(0);
	return 0;
}
