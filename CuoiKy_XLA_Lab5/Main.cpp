#include <opencv2/opencv.hpp>
#include "Kmean.h"
#include "Threshold.h"


using namespace cv;


int main() {


	Mat image = imread("lena.png");
	Mat destimage;

	Mat gray_scale;
	cv::cvtColor(image, gray_scale, cv::COLOR_BGR2GRAY);


	namedWindow("main");
	imshow("main", image);

	namedWindow("grayscale");
	imshow("grayscale", gray_scale);


	/********************************************************/
	int low = 60;
	int high = 150;
	StaticThreshold* static_threshold = new StaticThreshold(low, high);
	static_threshold->Apply(image, destimage);

	namedWindow("static_threshold");
	imshow("static_threshold", destimage);


	/********************************************************/

	Size mask_33 = Size(3, 3);
	Size mask_55 = Size(5, 5);
	Size mask_77 = Size(7, 7);
	Size mask_99 = Size(9, 9);
	int average_threshold_C = 7;

	AverageLocalThreshold* average_threshold = new AverageLocalThreshold(average_threshold_C);
	average_threshold->Apply(image, destimage, mask_77);

	namedWindow("average_threshold");
	imshow("average_threshold", destimage);
	

	/********************************************************/

	int median_threshold_C = 5;

	MedianLocalThreshold* median_threshold = new MedianLocalThreshold(median_threshold_C);
	median_threshold->Apply(image, destimage, mask_77);

	namedWindow("median_threshold");
	imshow("median_threshold", destimage);


	/********************************************************/

	int sauvo_threshold_r = 200;
	float sauvo_threshold_k = 0.5f;
	SauvolaLocalThreshold* sauvo_threshold = new SauvolaLocalThreshold(sauvo_threshold_r, sauvo_threshold_k);
	sauvo_threshold->Apply(image, destimage, mask_33);

	namedWindow("sauvo_threshold");
	imshow("sauvo_threshold", destimage);

	/********************************************************/

	int k_cluster = 12;
	Kmean* k_mean = new Kmean(k_cluster);
	k_mean->Apply(image, destimage);

	namedWindow("k_mean");
	imshow("k_mean", destimage);

	waitKey(0);
	return 0;
}