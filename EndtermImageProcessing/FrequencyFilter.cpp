#include "FrequencyFilter.h"

FrequencyFilter::FrequencyFilter(int minX, int maxX, int minY, int maxY)
{
	_minX = minX;
	_maxX = maxX;
	_minY = minY;
	_maxY = maxY;
}

FrequencyFilter::~FrequencyFilter()
{
}

void FrequencyFilter::Apply(ComplexImage& image)
{
	//Duyệt ảnh, những tần số nằm ngoài ngưỡng thì gán bằng 0
	int rows = image.dataRows, cols = image.dataCols;
	complex<double> *pRow = image.data;

	for (int i = 0; i < rows; i++, pRow += cols)
	{
		complex<double> *pData = pRow;
		for (int j = 0; j < cols; j++, pData++)
			if (i < _minX || i > _maxX || j < _minY || j > _maxY)
				*pData = 0;
	}
}
