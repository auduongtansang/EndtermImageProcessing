#include "FourierTransform.h"

FourierTransform::FourierTransform()
{
}

FourierTransform::~FourierTransform()
{
}

complex<double>* FourierTransform::FFT(complex<double> *signal, int cols, Direction direction)
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

	//Nếu số mẫu là 1, không cần biến đổi
	if (cols == 1)
		return signal;

	/*
	Chia đôi tín hiệu gốc thành 2 tín hiệu nhỏ hơn:
		- signalEven: chứa các mẫu tại vị trí chẵn của tín hiệu gốc
		- signalOdd: chứa các mẫu tại vị trí lẻ của tín hiệu gốc
	*/
	int half = cols / 2;
	complex<double> *signalEven = new complex<double>[half];
	complex<double> *signalOdd = new complex<double>[half];

	for (int i = 0; i < half; i++)
	{
		signalEven[i] = signal[2 * i];
		signalOdd[i] = signal[2 * i + 1];
	}

	//Gọi đệ quy biến đổi Fourier trên 2 tín hiệu này
	complex<double> *freqEven = FFT(signalEven, half, direction);
	complex<double> *freqOdd = FFT(signalOdd, half, direction);

	//Tính toán kết quả theo công thức đã chứng minh
	complex<double> *freqFinal = new complex<double>[cols];

	if (direction == Direction::Forward)
	{
		//Biến đổi thuận
		for (int k = 0; k < half; k++)
		{
			//Số hạng bên phải của công thức = hệ số * F_của_các_mẫu_tại_vị_trí_lẻ
			complex<double> rightArg = polar(1.0, -2.0 * M_PI * k / cols) * freqOdd[k];

			/*
			F[k] = F_của_các_mẫu_tại_vị_trí_chẵn[k] + hệ số * F_của_các_mẫu_tại_vị_trí_lẻ[k]
			Miền tần số lặp lại nhưng nghịch dấu ở các mẫu tại vị trí lẻ
			*/
			freqFinal[k] = freqEven[k] + rightArg;
			freqFinal[k + half] = freqEven[k] - rightArg;
		}
	}
	else
	{
		//Biến đổi ngược
		for (int k = 0; k < half; k++)
		{
			//Số hạng bên phải của công thức = hệ số * X_của_các_tần_số_tại_vị_trí_lẻ
			complex<double> rightArg = polar(1.0, 2.0 * M_PI * k / cols) * freqOdd[k];

			/*
			X[k] = X_của_các_tần_số_tại_vị_trí_chẵn[k] + hệ số * X_của_các_tần_số_tại_vị_trí_lẻ[k]
			Miền không gian cũng lặp lại nhưng nghịch dấu ở các tần số tại vị trí lẻ
			*/
			freqFinal[k] = freqEven[k] + rightArg;
			freqFinal[k + half] = freqEven[k] - rightArg;
		}
	}

	delete[] signalEven;
	delete[] signalOdd;
	return freqFinal;
}

complex<double>* FourierTransform::FFT2(complex<double> *signal, int rows, int cols, Direction direction)
{
	/*
	Biến đổi Fourier rời rạc cho tín hiệu 2D
	Biến đổi từng dòng, sau đó biến đổi từng cột
	*/
	complex<double> *freq = new complex<double>[rows * cols];

	//Biến đổi dòng
	for (int i = 0; i < rows; i++)
	{
		//Lấy dòng thứ i
		complex<double> *freqRow = new complex<double>[cols];
		memcpy(freqRow, signal + i * cols, cols * sizeof(complex<double>));

		//Biến đổi và gán vào kết quả
		freqRow = FFT(freqRow, cols, direction);
		memcpy(freq + i * cols, freqRow, cols * sizeof(complex<double>));

		delete[] freqRow;
	}

	//Biến đổi cột
	for (int i = 0; i < cols; i++)
	{
		//Lấy cột thứ i
		complex<double> *freqCol = new complex<double>[rows];
		for (int j = 0; j < rows; j++)
			freqCol[j] = freq[j * cols + i];

		//Biến đổi
		freqCol = FFT(freqCol, rows, direction);

		//Gán vào kết quả
		for (int j = 0; j < rows; j++)
			freq[j * cols + i] = freqCol[j];

		delete[] freqCol;
	}

	//Nếu là biến đổi nghịch, chia tất cả cho tổng số mẫu
	if (direction == Direction::Backward)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				freq[i * cols + j] *= 1.0 / (rows * cols);
	}

	return freq;
}
