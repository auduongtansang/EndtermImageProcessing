#include "ComplexImage.h"

ComplexImage::ComplexImage()
{
	data.clear();
	rows = cols = 0;
	isFourierTransformed = false;
}

ComplexImage::~ComplexImage()
{
	data.clear();
}

void ComplexImage::FromOpenCVMat(Mat image)
{
	//Kích thước của data phải là lũy thừa của 2, sử dụng zero-padding
	rows = image.rows;
	cols = image.cols;

	int dataRows = 2, dataCols = 2;
	while (dataRows < rows)
		dataRows *= 2;
	while (dataCols < cols)
		dataCols *= 2;

	//Khởi tạo toàn bộ data bằng 0
	data = vector<vector<complex<double>>>(dataRows, vector<complex<double>>(dataCols, 0));

	//Duyệt ảnh, đưa dữ liệu vào data
	uchar* pRow = image.data;
	for (int i = 0; i < rows; i++, pRow += cols)
	{
		uchar* pData = pRow;
		for (int j = 0; j < cols; j++, pData++)
			data[i][j] = complex<double>(*pData, 0) * pow(-1.0, i + j);
	}
}

Mat ComplexImage::ToOpenCVMat()
{
	Mat image;

	//Nếu data đang ở trạng thái tần số, trả về ảnh biên độ
	if (isFourierTransformed == true)
	{
		int dataRows = (int)(data.size()), dataCols = (int)(data[0].size());
		image = Mat(dataRows, dataCols, CV_8UC1, Scalar(0));

		//Tính tất cả biên độ và tìm biên độ lớn nhất
		vector<vector<double>> magnitude(dataRows, vector<double>(dataCols));
		double maxMag = -9999999999999999999.0;
		for (int i = 0; i < dataRows; i++)
		{
			for (int j = 0; j < dataCols; j++)
			{
				magnitude[i][j] = norm(data[i][j]);
				if (maxMag < magnitude[i][j])
					maxMag = magnitude[i][j];
			}
		}

		//Log scale và lưu biên độ vào ảnh kết quả
		double c = 255.0 / log10(1.0 + maxMag);
		uchar* pRow = image.data;
		for (int i = 0; i < dataRows; i++, pRow += dataCols)
		{
			uchar* pData = pRow;
			for (int j = 0; j < dataCols; j++, pData++)
				*pData = (uchar)(round(c * log10(1.0 + magnitude[i][j])));
		}
	}
	//Ngược lại, nếu đang ở trạng thái không gian, trả về ảnh xám
	else
	{
		image = Mat(rows, cols, CV_8UC1, Scalar(0));

		//Duyệt và đưa phần thực của data vào ảnh kết quả
		uchar* pRow = image.data;
		for (int i = 0; i < rows; i++, pRow += cols)
		{
			uchar* pData = pRow;
			for (int j = 0; j < cols; j++, pData++)
				*pData = (uchar)(round(data[i][j].real() * pow(-1.0, i + j)));
		}
	}

	return image;
}

void ComplexImage::ForwardFourierTransform()
{
	FourierTransform transformer;
	data = transformer.FFT2(data, Direction::Forward);
	isFourierTransformed = true;
}

void ComplexImage::BackwardFourierTransform()
{
	FourierTransform transformer;
	data = transformer.FFT2(data, Direction::Backward);
	isFourierTransformed = false;
}