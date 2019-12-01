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
	vector<complex<double>> FFT(vector<complex<double>> signal, Direction direction);

	/*
	Hàm biến đổi Fourier nhanh cho tín hiệu rời rạc 2D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	vector<vector<complex<double>>> FFT2(vector<vector<complex<double>>> signal, Direction direction);
};