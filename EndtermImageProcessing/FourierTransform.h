#pragma once
#define _USE_MATH_DEFINES

#include <complex>
#include <math.h>
#include <vector>

using namespace std;

//Enum xác định chiều của biến đổi Fourier (thuận hay nghịch)
enum class Direction
{
	//Thuận
	Forward,

	//Nghịch
	Backward
};

class FourierTransform
{
public:
	FourierTransform();
	~FourierTransform();

	/*
	Hàm biến đổi Fourier nhanh cho tín hiệu rời rạc 1D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	complex<double>* FFT(complex<double> *signal, int cols, Direction direction);

	/*
	Hàm biến đổi Fourier nhanh cho tín hiệu rời rạc 2D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	complex<double>* FFT2(complex<double> *signal, int rows, int cols, Direction direction);
};