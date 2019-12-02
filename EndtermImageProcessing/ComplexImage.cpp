#include "ComplexImage.h"

ComplexImage::ComplexImage()
{
	data = nullptr;
	dataRows = dataCols = rows = cols = 0;
	isFourierTransformed = false;
}

ComplexImage::~ComplexImage()
{
	if (data != nullptr)
		delete[] data;
}

void ComplexImage::FromOpenCVMat(Mat image)
{
	//Kích thước của data phải là lũy thừa của 2, sử dụng zero-padding
	rows = image.rows;
	cols = image.cols;

	dataRows = 2;
	dataCols = 2;

	while (dataRows < rows)
		dataRows *= 2;
	while (dataCols < cols)
		dataCols *= 2;

	//Khởi tạo toàn bộ data bằng 0
	delete[] data;
	data = new complex<double>[dataRows * dataCols];
	memset(data, 0, dataRows * dataCols * sizeof(complex<double>));

	//Duyệt ảnh, đưa dữ liệu vào data
	uchar* pRow = image.data;
	for (int i = 0; i < rows; i++, pRow += cols)
	{
		uchar* pData = pRow;
		for (int j = 0; j < cols; j++, pData++)
			data[i * dataCols + j] = complex<double>(*pData, 0) * pow(-1.0, i + j);
	}
}

Mat ComplexImage::ToOpenCVMat()
{
	Mat image;

	//Nếu data đang ở trạng thái tần số, trả về ảnh biên độ
	if (isFourierTransformed == true)
	{
		image = Mat(dataRows, dataCols, CV_8UC1, Scalar(0));

		//Tính tất cả biên độ và tìm biên độ lớn nhất
		double* magnitude = new double[dataRows * dataCols];
		double maxMag = -9999999999999999999.0;

		for (int i = 0; i < dataRows; i++)
		{
			for (int j = 0; j < dataCols; j++)
			{
				magnitude[i * dataCols + j] = norm(data[i * dataCols + j]);
				if (maxMag < magnitude[i * dataCols + j])
					maxMag = magnitude[i * dataCols + j];
			}
		}

		//Log scale và lưu biên độ vào ảnh kết quả
		double c = 255.0 / log10(1.0 + maxMag);
		uchar* pRow = image.data;

		for (int i = 0; i < dataRows; i++, pRow += dataCols)
		{
			uchar* pData = pRow;
			for (int j = 0; j < dataCols; j++, pData++)
				*pData = (uchar)(round(c * log10(1.0 + magnitude[i * dataCols + j])));
		}

		//Resize kích thước cho giống với ảnh gốc
		resize(image, image, Size(cols, rows), 0, 0, 1);

		delete[] magnitude;
	}
	//Ngược lại, nếu data đang ở trạng thái không gian, trả về ảnh xám
	else
	{
		image = Mat(rows, cols, CV_8UC1, Scalar(0));

		//Duyệt và đưa phần thực của data vào ảnh kết quả
		uchar* pRow = image.data;
		for (int i = 0; i < rows; i++, pRow += cols)
		{
			uchar* pData = pRow;
			for (int j = 0; j < cols; j++, pData++)
				*pData = (uchar)(round(data[i * dataCols + j].real() * pow(-1.0, i + j)));
		}
	}

	return image;
}

void ComplexImage::ForwardFourierTransform()
{
	FourierTransform transformer;
	data = transformer.FFT2(data, dataRows, dataCols, Direction::Forward);
	isFourierTransformed = true;
}

void ComplexImage::BackwardFourierTransform()
{
	FourierTransform transformer;
	data = transformer.FFT2(data, dataRows, dataCols, Direction::Backward);
	isFourierTransformed = false;
}