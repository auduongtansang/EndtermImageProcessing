#include "Threshold.h"

int StaticThreshold::Apply(const Mat & srcImage, Mat & dstImage)
{
	if (srcImage.cols < 0 || srcImage.rows < 0)
		return 0;

	int nChannel = srcImage.channels();
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;

	uchar* black_color = NULL;
	uchar* grey_color = NULL;
	uchar* white_color = NULL;
	black_color = new uchar;
	*black_color = 0;

	grey_color = new uchar;
	*grey_color = 128;

	white_color = new uchar;
	*white_color = 255;


	//Convert to gray scale if source image is RGB 
	Mat gray_scale;

	if (nChannel == 3) {
		cvtColor(srcImage, gray_scale, cv::COLOR_BGR2GRAY);
	}
	else {
		gray_scale = srcImage.clone();
	}

	dstImage = Mat(src_rows, src_cols, CV_8UC1);
	uchar* p = gray_scale.data;
	uchar* p_dest = dstImage.data;

	//Local threshold
	for (int x = 0; x < src_rows; x++) {
		for (int y = 0; y < src_cols; y++) {

			if (*(p + x * src_cols + y) < this->_lowThreshold) {
				*(p_dest + x * src_cols + y) = *white_color;
			}
			else if (*(p + x * src_cols + y) > this->_highThreshold) {
				*(p_dest + x * src_cols + y) = *black_color;
			}
			else {
				*(p_dest + x * src_cols + y) = *grey_color;
			}
		}
	}

	return 1;
}

StaticThreshold::StaticThreshold()
{
	this->_lowThreshold = 0;
	this->_highThreshold = 255;
}

StaticThreshold::StaticThreshold(int low, int high)
{
	this->_lowThreshold = low;
	this->_highThreshold = high;
}

StaticThreshold::StaticThreshold(const StaticThreshold & D)
{
	this->_lowThreshold = D._lowThreshold;
	this->_highThreshold = D._highThreshold;
}

StaticThreshold::~StaticThreshold()
{
}

int AverageLocalThreshold::Apply(const Mat & srcImage, Mat & dstImage, Size winSize)
{
	if (srcImage.cols < 0 || srcImage.rows < 0)
		return 0;

	int nChannel = srcImage.channels();
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;

	int mask_height = winSize.height;
	int mask_width = winSize.width;

	uchar* black_color = NULL;
	uchar* white_color = NULL;
	black_color = new uchar;
	*black_color = 0;

	white_color = new uchar;
	*white_color = 255;

	//Convert to gray scale if source image is RGB 
	Mat gray_scale;

	if (nChannel == 3) {
		cvtColor(srcImage, gray_scale, cv::COLOR_BGR2GRAY);
	}
	else {
		gray_scale = srcImage.clone();
	}

	dstImage = Mat(src_rows, src_cols, CV_8UC1);
	uchar* p = gray_scale.data;
	uchar* p_dest = dstImage.data;
	
	//Average local threshold
	float mean;
	for (int x = 0; x < src_rows; x++) {
		for (int y = 0; y < src_cols; y++) {

			//Find mean of mask
			mean = (float)*(p + x * src_cols + y);
			for (int i = -(mask_height / 2); i <= mask_height / 2; i++) {
				for (int j = -(mask_width / 2); j <= mask_width / 2; j++) {
					if ((x + i) >= 0 && (x + i) < src_rows && (y + j) >= 0 && (y + j) < src_cols)
						mean += *(p + (x + i) * src_cols + (y + j));
				}
			}

			mean /= ((float)mask_height * (float)mask_width);

			//Local threshold
			if (*(p + x  * src_cols + y) > (mean - this->_C)) {
				*(p_dest + x * src_cols + y) = *black_color;
			}
			else {
				*(p_dest + x * src_cols + y) = *white_color;
			}
		}
	}
	return 1;
}

AverageLocalThreshold::AverageLocalThreshold()
{
	this->_C = 0;
}

AverageLocalThreshold::AverageLocalThreshold(int d_C)
{
	this->_C = d_C;
}

AverageLocalThreshold::AverageLocalThreshold(const AverageLocalThreshold & D)
{
	this->_C = D._C;
}

AverageLocalThreshold::~AverageLocalThreshold()
{
}

int MedianLocalThreshold::Apply(const Mat & srcImage, Mat & dstImage, Size winSize)
{
	if (srcImage.cols < 0 || srcImage.rows < 0)
		return 0;

	int nChannel = srcImage.channels();
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;

	int mask_height = winSize.height;
	int mask_width = winSize.width;

	uchar* black_color = NULL;
	uchar* white_color = NULL;
	black_color = new uchar;
	*black_color = 0;

	white_color = new uchar;
	*white_color = 255;

	//Convert to gray scale if source image is RGB 
	Mat gray_scale;

	if (nChannel == 3) {
		cvtColor(srcImage, gray_scale, cv::COLOR_BGR2GRAY);
	}
	else {
		gray_scale = srcImage.clone();
	}

	dstImage = Mat(src_rows, src_cols, CV_8UC1);
	uchar* p = gray_scale.data;
	uchar* p_dest = dstImage.data;

	//Average local threshold
	vector<int> list;
	int median;

	for (int x = 0; x < src_rows; x++) {
		for (int y = 0; y < src_cols; y++) {
			
			//Find median in mask
			list.clear();
			median = *(p + x * src_cols + y);
			for (int i = -(mask_height / 2); i <= mask_height / 2; i++) {
				for (int j = -(mask_width / 2); j <= mask_width / 2; j++) {
					if ((x + i) >= 0 && (x + i) < src_rows && (y + j) >= 0 && (y + j) < src_cols)
						list.push_back(*(p + (x + i) * src_cols + (y + j)));
				}
			}

			sort(list.begin(), list.end());

			if ((list.size() & 1) == 0) {
				median = (list[list.size() / 2] + list[list.size() / 2 + 1]) / 2;
			}
			else {
				median = list[list.size() / 2];
			}

			//Local threshold
			if (*(p + x * src_cols + y) > (median - this->_C)) {
				*(p_dest + x * src_cols + y) = *black_color;
			}
			else {
				*(p_dest + x * src_cols + y) = *white_color;
			}
		}
	}
	return 1;
}

MedianLocalThreshold::MedianLocalThreshold()
{
}

MedianLocalThreshold::MedianLocalThreshold(int d_C)
{
	this->_C = d_C;
}

MedianLocalThreshold::MedianLocalThreshold(const MedianLocalThreshold & D)
{
	this->_C = D._C;
}

MedianLocalThreshold::~MedianLocalThreshold()
{
}

int SauvolaLocalThreshold::Apply(const Mat & srcImage, Mat & dstImage, Size winSize)
{
	if (srcImage.cols < 0 || srcImage.rows < 0)
		return 0;

	int nChannel = srcImage.channels();
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;

	int mask_height = winSize.height;
	int mask_width = winSize.width;

	uchar* black_color = NULL;
	uchar* white_color = NULL;
	black_color = new uchar;
	*black_color = 0;

	white_color = new uchar;
	*white_color = 255;

	//Convert to gray scale if source image is RGB 
	Mat gray_scale;

	if (nChannel == 3) {
		cvtColor(srcImage, gray_scale, cv::COLOR_BGR2GRAY);
	}
	else {
		gray_scale = srcImage.clone();
	}

	dstImage = Mat(src_rows, src_cols, CV_8UC1);
	uchar* p = gray_scale.data;
	uchar* p_dest = dstImage.data;

	//Average local threshold
	vector<int> list;
	float standard_deviation;
	float mean;
	for (int x = 0; x < src_rows; x++) {
		for (int y = 0; y < src_cols; y++) {

			//Find mean and standard deviation in mask
			list.clear();
			standard_deviation = 0.0f;
			mean = (float)*(p + x * src_cols + y);

			for (int i = -(mask_height / 2); i <= mask_height / 2; i++) {
				for (int j = -(mask_width / 2); j <= mask_width / 2; j++) {
					if ((x + i) >= 0 && (x + i) < src_rows && (y + j) >= 0 && (y + j) < src_cols) {
						list.push_back(*(p + (x + i) * src_cols + (y + j)));
						mean += *(p + (x + i) * src_cols + (y + j));
					}
				}
			}

			mean /= ((float)mask_height * (float)mask_width);

			for (int i = 0; i < list.size(); i++) {
				standard_deviation += ((list[i] - mean) * (list[i] - mean));
			}
			standard_deviation /= list.size();
			
			//Local threshold
			if (*(p + x  * src_cols + y) > (mean * (1 + this->_k * (standard_deviation / this->_r - 1)))) {
				*(p_dest + x * src_cols + y) = *black_color;
			}
			else {
				*(p_dest + x * src_cols + y) = *white_color;
			}
		}
	}
	return 1;
}

SauvolaLocalThreshold::SauvolaLocalThreshold()
{
}

SauvolaLocalThreshold::SauvolaLocalThreshold(int d_r, float d_k)
{
	this->_r = d_r;
	this->_k = d_k;
}

SauvolaLocalThreshold::SauvolaLocalThreshold(const SauvolaLocalThreshold & D)
{
	this->_k = D._k;
	this->_r = D._r;
}

SauvolaLocalThreshold::~SauvolaLocalThreshold()
{
}
