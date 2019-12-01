#include <iostream>
#include <complex>
#include <vector>
#include "FourierTransform.h"

using namespace std;

int main()
{
	//Khởi tạo ma trận kiểm thử
	vector<vector<complex<double>>> mat(4);
	for (int i = 0; i < 4; i++)
		mat[i] = vector<complex<double>>(4, 0);
	mat[1][1] = mat[1][2] = mat[2][1] = mat[2][2] = 1;

	//Biến đổi thuận
	FourierTransform transformer;
	mat = transformer.FFT2(mat, Direction::Forward);

	//In kết quả
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << mat[i][j] << ' ';
		cout << endl;
	}

	//Biến đổi nghịch
	mat = transformer.FFT2(mat, Direction::Backward);

	//In kết quả
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << mat[i][j] << ' ';
		cout << endl;
	}

	return 0;
}