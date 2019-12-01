#include "ComplexImage.h"

ComplexImage::ComplexImage()
{
	data.clear();
	rows = cols = 0;
	isFourierTransformed = false;
}

ComplexImage::~ComplexImage()
{
}

void ComplexImage::FromOpenCVMat(Mat image)
{
}

Mat ComplexImage::ToOpenCVMat()
{
	return Mat();
}

void ComplexImage::ForwardFourierTransform()
{
}

void ComplexImage::BackwardFourierTransform()
{
}