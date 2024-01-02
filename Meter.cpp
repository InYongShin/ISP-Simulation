
#include "Meterer.hpp"

const float pi = 3.141592f;


static inline float Gauss2(int x, int y, float sigma2) {
	float a = 1 / (2 * pi * sigma2);
	float b = exp(-((x * x) + (y * y)) / (2 * sigma2));
	return a * b;
}


void Meterer::spotMeter(int x, int y) {

	cv::namedWindow("Metering", 0);
	cv::setMouseCallback("Metering", mouseCallback, this);

	float* copyData = new float[width*height*channels];
	memcpy(copyData, data, width*height*channels*sizeof(float));

	cv::Mat image = cv::Mat(cv::Size(width, height), CV_32FC3, copyData);
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

	for (int i=0; i<image.rows; ++i) for (int j=0; j<image.cols; ++j) {
		float r = image.at<cv::Vec3f>(i, j)[2];
		float g = image.at<cv::Vec3f>(i, j)[1];
		float b = image.at<cv::Vec3f>(i, j)[0];

		image.at<cv::Vec3f>(i, j)[2] = pow(r, 1/2.2f);
		image.at<cv::Vec3f>(i, j)[1] = pow(g, 1/2.2f);
		image.at<cv::Vec3f>(i, j)[0] = pow(b, 1/2.2f);
	}

	cv::imshow("Metering", image);
	cv::waitKey();

	printf("Spot point: %d %d\n", moustPtX, moustPtY);


	float sum = 0.f;
	int excludedCnt = 0;
	for(int i=-y/2; i<y/2; ++i) for(int j=-x/2; j<x/2; ++j){
		int yPos = moustPtY - i;
		int xPos = moustPtX - j;

		if(yPos<0 || yPos>height-1 || xPos<0 || xPos>width-1){
			excludedCnt++;
			continue;
		}

		int index = 3*yPos*width + 3*xPos;
		sum += (0.2126*data[index] + 0.7152*data[index+1] + 0.0722*data[index+2]);
	}
	sum /= (2*(y/2) * 2*(x/2) - excludedCnt);
	printf("Average brightness: %f\n", sum);

	float ratio = .5f / sum;
	if(ratio > 16) ratio = 16;

	for(int i=0; i<height; ++i) for(int j=0; j<width; ++j) for(int k=0; k<channels; ++k) {
		int index = channels*i*width + channels*j + k;
		data[index] *= ratio;
		if(data[index] > 1.f) data[index] = 1.f;
	}

	delete[] copyData;
	bMetered = true;
}


void Meterer::centerWeightedMeter() {

	int startW = width * 10 / 100;
	int endW = width * 90 / 100;
	int startH = height * 10 / 100;
	int endH = height * 90 / 100;

	float sum = 0.f;
	float sigma2 = 1000 * 1000;
	for(int i=startH; i<endH; ++i) for(int j=startW; j<endW; ++j){
		int index = 3*i*width + 3*j;

		float gauss = Gauss2(j-(width/2), i-(height/2), sigma2) * sigma2 * 3;
		if (gauss > 1.f) gauss = 1.f;
		float r = data[index  ] * gauss;
		float g = data[index+1] * gauss;
		float b = data[index+2] * gauss;
		/*data[index] = gauss;
		data[index+1] = gauss;
		data[index+2] = gauss;*/
		sum += (0.2126*r + 0.7152*g + 0.0722*b);
	}
	sum /= (endH-startH) * (endW-startW);
	printf("Average brightness: %f\n", sum);

	float ratio = .5f / sum;
	if(ratio > 4) ratio = 4;

	for(int i=0; i<height; ++i) for(int j=0; j<width; ++j) for(int k=0; k<channels; ++k) {
		int index = channels*i*width + channels*j + k;
		data[index] *= ratio;
		if(data[index] > 1.f) data[index] = 1.f;
	}

	bMetered = true;
}