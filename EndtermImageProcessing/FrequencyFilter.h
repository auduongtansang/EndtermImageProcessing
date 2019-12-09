#pragma once
#include <complex>
#include "ComplexImage.h"

class FrequencyFilter
{
	//Ngưỡng tần số giữ lại (giữ lại các tần số trong khoảng [min, max])
	int _minX, _maxX, _minY, _maxY;

public:
	//Hàm khởi tạo, mặc định min = 0, max = 1024
	FrequencyFilter(int minX = 0, int maxX = 1024, int minY = 0, int maxY = 1024);
	~FrequencyFilter();

	//Hàm áp dụng phép lọc lên một ảnh phức ở dạng tần số
	void Apply(ComplexImage &image);
};
