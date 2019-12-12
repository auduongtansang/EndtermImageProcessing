#include "CannyEdgeDetector.h"

void CannyEdgeDetector::Set(int Min, int Max)
{
	_lowThreshold = Min;
	_highThreshold = Max;
}

bool CannyEdgeDetector::Exist(Mat S, int x, int y)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (S.at<short>(x + i, y + j) >= _highThreshold)
			{
				return true;
			}
		}
	}
	return false;
}

int CannyEdgeDetector::Apply(const Mat& srcImage, Mat& dstImage)
{
	if (empty(srcImage))
	{
		return 0;
	}

	Mat Gauss(srcImage.rows, srcImage.cols, CV_8UC1, Scalar(0));
	Blur B;
	//Làm mờ ảnh bằng Gauss
	B.BlurImage(srcImage, Gauss, 5, 5, 2);

	Mat S(srcImage.rows, srcImage.cols, CV_16SC1, Scalar(0)); //Ảnh đạo hàm của ảnh Gauss
	Mat angle(srcImage.rows, srcImage.cols, CV_32FC1, Scalar(0)); //Ma trận góc

	//Tính đạo hàm theo hướng x, y sau khi làm trơn
	Convolution convolution;
	vector<int> kernelX;
	vector<int> kernelY;
	float division = 1.0f;
	Mat dx, dy;

	/*
		|1  0  -1|   1
		|2  0  -2| * -  =  Kx
		|1  0  -1|   4
	*/
	kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

	kernelX.push_back(2); kernelX.push_back(0); kernelX.push_back(-2);

	kernelX.push_back(1); kernelX.push_back(0); kernelX.push_back(-1);

	/*
		|-1  -2  -1|   1
		| 0   0   0| * -  =  Ky
		| 1   2   1|   4
	*/

	kernelY.push_back(-1); kernelY.push_back(-2); kernelY.push_back(-1);

	kernelY.push_back(0); kernelY.push_back(0); kernelY.push_back(0);

	kernelY.push_back(1); kernelY.push_back(2); kernelY.push_back(1);

	division = 4.0f;

	convolution.SetKernel(kernelX, division, 3, 3);
	convolution.DoConvolution(Gauss, dx);

	convolution.SetKernel(kernelY, division, 3, 3);
	convolution.DoConvolution(Gauss, dy);

	//Tính ma trận góc:
	for (int i = 1; i < angle.rows - 1; i++)
	{
		for (int j = 1; j < angle.cols - 1; j++)
		{
			short a = dy.at<short>(i, j);
			short b = dx.at<short>(i, j);

			if (a > 0 && b == 0)
			{
				angle.at<float>(i, j) = 90;
			}
			else if (a < 0 && b == 0)
			{
				angle.at<float>(i, j) = -90;
			}
			else if (a == 0 && b == 0)
			{
				angle.at<float>(i, j) = 0;
			}
			else
			{
				angle.at<float>(i, j) = (float)(atan(a / b) * 180 / PI);
			}
		}
	}

	//Tính S(x, y) = sqrt(Sx ^ 2 + Sy ^ 2)
	for (int i = 0; i < S.rows; i++)
	{
		for (int j = 0; j < S.cols; j++)
		{
			S.at<short>(i, j) = (short)sqrt(dx.at<short>(i, j) * dx.at<short>(i, j) + dy.at<short>(i, j) * dy.at<short>(i, j));
		}
	}

	//Chuẩn hóa về 4 bin: 0 | 45 | 90 | 135
	for (int i = 1; i < angle.rows - 1; i++)
	{
		for (int j = 1; j < angle.cols - 1; j++)
		{
			float a = angle.at<float>(i, j);

			if (a <= 22.5 || a > 157.5 || a >= -22.5 || a < -157.5)
			{
				angle.at<float>(i, j) = 0;
			}
			else if ((a > 22.5 && a <= 67.5) || (a > -157.5 && a <= -112.5))
			{
				angle.at<float>(i, j) = 45;
			}
			else if ((a > 67.5 && a <= 112.5) || (a > -112.5 && a <= -67.5))
			{
				angle.at<float>(i, j) = 90;
			}
			else if ((a > 112.5 && a <= 157.5) || (a > -67.5 && a <= -22.5))
			{
				angle.at<float>(i, j) = 135;
			}
		}
	}

	//Loại trừ dựa trên các điểm lân cận và hướng đạo hàm
	for (int i = 1; i < angle.rows - 1; i++)
	{
		for (int j = 1; j < angle.cols - 1; j++)
		{
			int a = (int)angle.at<float>(i, j);

			if (a == 0)
			{
				short s1 = S.at<short>(i, j + 1);
				short s2 = S.at<short>(i, j - 1);
				short s = S.at<short>(i, j);
				if (s != MAX(MAX(s, s1), s2))
				{
					S.at<short>(i, j) = 0;
				}
			}
			else if (a == 45)
			{
				short s1 = S.at<short>(i + 1, j - 1);
				short s2 = S.at<short>(i - 1, j + 1);
				short s = S.at<short>(i, j);
				if (s != MAX(MAX(s, s1), s2))
				{
					S.at<short>(i, j) = 0;
				}
			}
			else if (a == 90)
			{
				short s1 = S.at<short>(i - 1, j);
				short s2 = S.at<short>(i + 1, j);
				short s = S.at<short>(i, j);
				if (s != MAX(MAX(s, s1), s2))
				{
					S.at<short>(i, j) = 0;
				}
			}
			else if (a == 135)
			{
				short s1 = S.at<short>(i - 1, j - 1);
				short s2 = S.at<short>(i + 1, j + 1);
				short s = S.at<short>(i, j);
				if (s != MAX(MAX(s, s1), s2))
				{
					S.at<short>(i, j) = 0;
				}
			}
		}
	}

	//Lọc ngưỡng
	Mat DesImage(S.rows, S.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < S.rows; i++)
	{
		for (int j = 0; j < S.cols; j++)
		{
			if (S.at<short>(i, j) >= _lowThreshold && S.at<short>(i, j) <= _highThreshold)
			{
				if (Exist(S, i, j) == true)
				{
					DesImage.at<uchar>(i, j) = 255;
				}
			}
			else if (S.at<short>(i, j) >= _highThreshold)
			{
				DesImage.at<uchar>(i, j) = 255;
			}
			else
			{
				DesImage.at<uchar>(i, j) = 0;
			}
		}
	}
	dstImage = DesImage;
	return 1;
}

CannyEdgeDetector::CannyEdgeDetector()
{
	_lowThreshold = 0;
	_highThreshold = 9999;
}

CannyEdgeDetector::~CannyEdgeDetector()
{
}
