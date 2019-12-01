﻿#include "FourierTransform.h"

FourierTransform::FourierTransform()
{
}

FourierTransform::~FourierTransform()
{
}

vector<complex<double>> FourierTransform::FFT(vector<complex<double>> signal, Direction direction)
{
	/*
	Trong biến đổi Fourier rời rạc, có:
		F[k] = sum(X[i] * exp(-j * 2 * pi * k * i / n))
		(với X là tín hiệu rời rạc có n mẫu, j là đơn vị ảo, 0 <= i, k <= n)

		Chứng minh được:
			F[k] = sum(X[u] * exp(-j * 2 * pi * k * u / m)) + exp(-j * 2 * pi * k / n) * sum(X[v] * exp(-j * 2 * pi * k * v / m))
			(với m = n / 2, u là số chẵn, v là số lẻ, 0 <= u, v, k <= n)

		Do đó:
			F[k] = F_của_các_mẫu_tại_vị_trí_chẵn[k] + hệ số * F_của_các_mẫu_tại_vị_trí_lẻ[k]
			=> Gọi đệ quy để tính toán.

		Hơn nữa, với tín hiệu có n mẫu, miền tần số sẽ lặp lại sau mỗi n / 2 mẫu.

	Tương tự cho phép biến đổi nghịch.
	*/

	int n = (int)(signal.size());

	//Nếu số mẫu là 1, không cần biến đổi
	if (n == 1)
		return signal;

	/*
	Chia đôi tín hiệu gốc thành 2 tín hiệu nhỏ hơn:
		- signalEven: chứa các mẫu tại vị trí chẵn của tín hiệu gốc
		- signalOdd: chứa các mẫu tại vị trí lẻ của tín hiệu gốc
	*/
	int half = n / 2;
	vector<complex<double>> signalEven(half);
	vector<complex<double>> signalOdd(half);

	for (int i = 0; i < half; i++)
	{
		signalEven[i] = signal[2 * i];
		signalOdd[i] = signal[2 * i + 1];
	}

	//Gọi đệ quy biến đổi Fourier trên 2 tín hiệu này
	vector<complex<double>> freqEven = FFT(signalEven, direction);
	vector<complex<double>> freqOdd = FFT(signalOdd, direction);

	//Tính toán kết quả theo công thức đã chứng minh
	vector<complex<double>> freqFinal(n);

	if (direction == Direction::Forward)
	{
		//Biến đổi thuận
		for (int i = 0; i < half; i++)
		{
			//Số hạng bên phải của công thức = hệ số * F_của_các_mẫu_tại_vị_trí_lẻ
			complex<double> rightArg = polar(1.0, -2.0 * M_PI * i / n) * freqOdd[i];

			/*
			F[k] = F_của_các_mẫu_tại_vị_trí_chẵn[k] + hệ số * F_của_các_mẫu_tại_vị_trí_lẻ[k]
			Miền tần số lặp lại nhưng nghịch dấu ở các mẫu tại vị trí lẻ
			*/
			freqFinal[i] = freqEven[i] + rightArg;
			freqFinal[i + half] = freqEven[i] - rightArg;
		}
	}
	else
	{
		//Biến đổi ngược
		for (int i = 0; i < half; i++)
		{
			//Số hạng bên phải của công thức = hệ số * X_của_các_tần_số_tại_vị_trí_lẻ
			complex<double> rightArg = polar(1.0, -2.0 * M_PI * i / n) * freqOdd[i];

			/*
			X[k] = X_của_các_tần_số_tại_vị_trí_chẵn[k] + hệ số * X_của_các_tần_số_tại_vị_trí_lẻ[k]
			Miền không gian cũng lặp lại nhưng nghịch dấu ở các tần số tại vị trí lẻ
			*/
			freqFinal[i] = freqEven[i] + rightArg;
			freqFinal[i + half] = freqEven[i] - rightArg;
		}
	}

	return freqFinal;
}

vector<vector<complex<double>>> FourierTransform::FFT2(vector<vector<complex<double>>> signal, Direction direction)
{
	/*
	Biến đổi Fourier rời rạc cho tín hiệu 2D
	Biến đổi từng dòng, sau đó biến đổi từng cột
	*/
	int rows = (int)(signal.size());
	int cols = (int)(signal[0].size());
	vector<vector<complex<double>>> freq(rows);

	//Biến đổi dòng
	for (int i = 0; i < rows; i++)
		freq[i] = FFT(signal[i], direction);

	//Biến đổi cột (chuyển vị => biến đổi dòng => chuyển vị)
	vector<vector<complex<double>>> freqT(cols, vector<complex<double>>(rows));
	for (int i = 0; i < cols; i++)
	{
		//Chuyển vị
		for (int j = 0; j < rows; j++)
			freqT[i][j] = freq[j][i];
		//Biến đổi dòng
		freqT[i] = FFT(freqT[i], direction);
	}
	//Chuyển vị lại vị trí cũ
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			freq[i][j] = freqT[j][i];

	//Nếu là biến đổi nghịch, chia tất cả cho tổng số mẫu
	if (direction == Direction::Backward)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				freq[i][j] *= 1.0 / (rows * cols);
	}

	return freq;
}