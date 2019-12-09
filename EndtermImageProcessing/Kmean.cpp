#include "Kmean.h"

int Kmean::Apply(const Mat & srcImage, Mat & dstImage)
{
	if (srcImage.cols < 0 || srcImage.rows < 0)
		return 0;

	int nChannel = srcImage.channels();
	int src_rows = srcImage.rows;
	int src_cols = srcImage.cols;
	uchar *p = srcImage.data;

	//Reshape the image
	Mat tmp = srcImage.reshape(0, src_rows*src_cols).clone();

	int tmp_rows = tmp.rows;
	int tmp_cols = tmp.cols;
	uchar* p_tmp = tmp.data;

	//Create destinate image
	if (nChannel == 3) {
		dstImage = Mat(src_rows, src_cols, CV_8UC3);
	}
	else {
		dstImage = Mat(src_rows, src_cols, CV_8UC1);
	}
	uchar* p_dest = dstImage.data;

	struct trplit {
		float x, y, z;
	};
	int index;

	//Random k centroids
	vector<trplit> centroids;

	for (int i = 0; i < this->_numClusters; i++) {

		index = rand() % tmp_rows;
		
		if (nChannel == 3)
			centroids.push_back({ (float)(*(p_tmp + index * 3)), (float)(*(p_tmp + index * 3 + 1)), (float)(*(p_tmp + index * 3 + 2)) });
		else
			centroids.push_back({ (float)(*(p_tmp + index * 3)), 0.0f, 0.0f });
	}

	
	//Prepare base construction for clustering processing
	vector<trplit> cluster;
	vector<std::vector<trplit>> clusters;
	clusters.resize(this->_numClusters);

	float min, dist;
	
	//fit
	for (int i = 0; i < 10; i++) {
		//Calculate all the distance from the pixel to the centroids and add them to the closest cluster
		for (int j = 0; j < tmp_rows; j++) {
			min = 0xffffffff;
			index = 0;

			for (int k = 0; k < this->_numClusters; k++) {
				if (nChannel == 3)
					dist = euclid_distance((float)(*(p_tmp + j * 3)), (float)(*(p_tmp + j * 3 + 1)), (float)(*(p_tmp + j * 3 + 2)),
						centroids[k].x, centroids[k].y, centroids[k].z);
				else
					dist = euclid_distance((float)(*(p_tmp + j)), 0.0f, 0.0f,
							centroids[k].x, centroids[k].y, centroids[k].z);
				if (min > dist) {
					min = dist;
					index = k;
				}
			}
			//Add data to the proper cluster
			if (nChannel == 3)
				clusters[index].push_back({ (float)(*(p_tmp + j * 3)), (float)(*(p_tmp + j * 3 + 1)), (float)(*(p_tmp + j * 3 + 2)) });
			else
				clusters[index].push_back({ (float)(*(p_tmp + j)), 0.0f, 0.0f });
		}

		//Calculate new centroid base on mean of cluster
		float average_x;
		float average_y;
		float average_z;

		for (int k = 0; k < this->_numClusters; k++) {
			//Sum all the data in one cluster per loop
			average_x = 0;
			average_y = 0;
			average_z = 0;

			for (int i = 0; i < clusters[k].size(); i++) {
				average_x += clusters[k][i].x;
				average_y += clusters[k][i].y;
				average_z += clusters[k][i].z;
			}

			//Divide to the number of cluster to get the mean of cluster
			if (clusters[k].size() > 0)
				average_x /= (float)clusters[k].size();
			if (clusters[k].size() > 0)
				average_y /= (float)clusters[k].size();
			if (clusters[k].size() > 0)
				average_z /= (float)clusters[k].size();

			//Replace the old centroids to the average ones which have been calculated
			centroids[k] = { average_x, average_y, average_z };
		}
	}

	//Predict
	for (int x = 0; x < src_rows; x++) {
		for (int y = 0; y < src_cols; y++) {
			min = 0xffffffff;
			index = 0;

			for (int k = 0; k < this->_numClusters; k++) {
				//Caculate the distance of our pixel to the centroids have been found
				if (nChannel == 3)
					dist = euclid_distance((float)(*(p + x * src_cols * nChannel + y * nChannel)), 
						(float)(*(p + x * src_cols  *nChannel + y * nChannel + 1)), 
						(float)(*(p + x * src_cols*nChannel + y * nChannel + 2)), 
					centroids[k].x, centroids[k].y, centroids[k].z);
				else
					dist = euclid_distance((float)(*(p + x * src_cols + y)), 0.0f, 0.0f,
						centroids[k].x, centroids[k].y, centroids[k].z);

				if (min > dist) {
					min = dist;
					index = k;
				}
			}

			if (nChannel == 3) {
				*(p_dest + x * src_cols * nChannel + y * nChannel + 0) = centroids[index].x;
				*(p_dest + x * src_cols*nChannel + y * nChannel + 1) = centroids[index].y;
				*(p_dest + x * src_cols*nChannel + y*nChannel + 2) = centroids[index].z;
			}
			else {
				*(p_dest + x * src_cols + y) = centroids[index].x;
			}
		}
		
	}

	return 1;
}

Kmean::Kmean()
{
	this->_numClusters = 1;
}

Kmean::Kmean(int d)
{
	this->_numClusters = d;
}

Kmean::Kmean(const Kmean & D)
{
	this->_numClusters = D._numClusters;
}

Kmean::~Kmean()
{
}

float euclid_distance(float ax, float ay, float az, float bx, float by, float bz)
{
	float d = (bx - ax) * (bx - ax) + (by - ay) * (by - ay)  + (bz - az) * (bz - az);

	if (az == 0.0f && bz == 0.0f)
		return sqrt(d);

	return pow(d, 1.0 / 3);
}
