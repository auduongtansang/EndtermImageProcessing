#pragma once
#include <complex>
#include <vector>

using namespace std;

class FourierTransform
{
public:
	FourierTransform();
	~FourierTransform();

	/*
	Hàm biến đổi Fourier nhanh (thuận) cho tín hiệu rời rạc 1D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	vector<complex<double>> FFT(vector<complex<double>> signal);

	/*
	Hàm biến đổi Fourier nhanh (nghịch) cho tín hiệu rời rạc 1D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	vector<complex<double>> IFFT(vector<complex<double>> signal);

	/*
	Hàm biến đổi Fourier nhanh (thuận) cho tín hiệu rời rạc 2D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	vector<complex<double>> FFT2(vector<complex<double>> signal);

	/*
	Hàm biến đổi Fourier nhanh (nghịch) cho tín hiệu rời rạc 2D
	Số mẫu của mỗi chiều trong tín hiệu phải là lũy thừa của 2
	*/
	vector<complex<double>> IFFT2(vector<complex<double>> signal);
};