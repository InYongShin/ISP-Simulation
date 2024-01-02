#ifndef ColorConvertor_hpp
#define ColorConvertor_hpp

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <Eigen/Dense>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <fstream>

// #include "GLTools/Window.hpp"


class ColorConvertor {
private:
	cv::Mat image;
	// cv::Mat recImage;
	// Window* win = nullptr;
	bool g_bDrawingBox = false;
	cv::Rect g_rect = cv::Rect(0,0,0,0);
	Eigen::MatrixXf ref = Eigen::MatrixXf(24, 3);
	Eigen::Matrix3f conversionMat;
	std::vector<Eigen::Vector3f> colors;

public:
	float* data = nullptr;
	int width=0, height=0;
	int channels = 3;
	bool bConverted = false;

	void checkerLoad(const std::string& fn) {
		// buf = (uint16_t*)stbi_load_16(fn.c_str(), &colorchecker.width, &colorchecker.height, &colorchecker.channels, 0);
		// image = cv::Mat(cv::Size(colorchecker.width, colorchecker.height), CV_16UC3, buf);
		
		// buf = (uint8_t*)stbi_load(fn.c_str(), &colorchecker.width, &colorchecker.height, &colorchecker.channels, 0);
		// image = cv::Mat(cv::Size(colorchecker.width, colorchecker.height), CV_8UC3, buf);

		// image = cv::imread(fn, -1);
	}

	void selectPatch() {
		cv::namedWindow("Patch", 0);
		cv::setMouseCallback("Patch", mouseCallback, this);

		image = cv::Mat(cv::Size(width, height), CV_32FC3, data);
		cv::Mat copyImage = cv::Mat::zeros(image.size(), CV_32FC3);
		image.copyTo(copyImage);

		for(int i=0; i<copyImage.rows; ++i) for(int j=0; j<copyImage.cols; ++j) {
			float r = copyImage.at<cv::Vec3f>(i,j)[2];
			float g = copyImage.at<cv::Vec3f>(i,j)[1];
			float b = copyImage.at<cv::Vec3f>(i,j)[0];

			copyImage.at<cv::Vec3f>(i,j)[2] = pow(r, 1/2.2f);
			copyImage.at<cv::Vec3f>(i,j)[1] = pow(g, 1/2.2f);
			copyImage.at<cv::Vec3f>(i,j)[0] = pow(b, 1/2.2f);
		}

		cv::imshow("Patch", copyImage);
		cv::waitKey();
		cv::destroyWindow("Patch");
	}

	/*void drawRectangle(const cv::Rect& box) {
		if (recImage.empty()) image.copyTo(recImage);
		cv::rectangle(recImage, box.tl(), box.br(), cv::Scalar(0, 0, 255));
		cv::imshow("Patch", recImage);
	}*/

	void averageColor(const cv::Rect& box) {
		cv::Point start = box.tl();
		cv::Point end = box.br();
		
		Eigen::Vector3f color = Eigen::Vector3f(0,0,0);
		for(int y=start.y+1; y<end.y; ++y){
			for(int x=start.x+1; x<end.x; ++x){
				color[0] += image.at<cv::Vec3f>(y, x)[0];
				color[1] += image.at<cv::Vec3f>(y, x)[1];
				color[2] += image.at<cv::Vec3f>(y, x)[2];
			}
		}
		color /= ((end.x-start.x-1)*(end.y-start.y-1));
		printf("Averaged color: %f, %f, %f\n", color[0], color[1], color[2]);
		colors.push_back(color);
	}

	void getConversionMat(const std::string& fn) {
		std::ifstream fin(fn);
		if (fin.peek() == std::ifstream::traits_type::eof()) {
			fprintf(stderr, "Not found file: %s\n", fn.c_str());
			return;
		}

		for(int i=0; i<3; ++i) for(int j=0; j<3; ++j){
			float value = 0.f;
			fin >> value;
			conversionMat(i, j) = value;
		}
		std::cout << "Conversion Matirx\n" << conversionMat << std::endl;
		fin.close();
	}

	void computeConversionMat() {
		Eigen::MatrixXf A(24, 3);
		for(int i=0; i<colors.size(); ++i){
			A.row(i) << colors[i].x(), colors[i].y(), colors[i].z();
		}
		A.normalize();
		ref.normalize();
		conversionMat = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ref);
		
		std::cout << "Conversion Matirx\n" << conversionMat << std::endl;
	}

	void colorConvert() {
		printf("Converting color ...\n");
		// convertedImage = cv::Mat(cv::Size(image.cols, image.rows), CV_8UC3, 0);
		// convertedImage = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
		// convertedBuf = new uint8_t[3*image.rows*image.cols];

		for(int y=0; y<height; ++y) for(int x=0; x<width; ++x) {
			Eigen::RowVector3f pixel;
			int index = 3*y*width + 3*x;
			pixel << data[index], data[index+1], data[index+2];
			
			Eigen::RowVector3f convertedPixel = pixel * conversionMat;
			
			if (convertedPixel(0) > 1) convertedPixel(0) = 1;
			if (convertedPixel(1) > 1) convertedPixel(1) = 1;
			if (convertedPixel(2) > 1) convertedPixel(2) = 1;
			if (convertedPixel(0) < 0) convertedPixel(0) = 0;
			if (convertedPixel(1) < 0) convertedPixel(1) = 0;
			if (convertedPixel(2) < 0) convertedPixel(2) = 0;

			data[index  ] = convertedPixel(0);
			data[index+1] = convertedPixel(1);
			data[index+2] = convertedPixel(2);
		}
		bConverted = true;
	}

	void saveConversionMat(const std::string& fn) {
		std::ofstream fout(fn);

		if (!fout) {
			fprintf(stderr, "Not found file: %s\n", fn.c_str());
			return;
		}

		fout << conversionMat(0,0) << " " << conversionMat(0,1) << " " << conversionMat(0,2) << "\n"
			 << conversionMat(1,0) << " " << conversionMat(1,1) << " " << conversionMat(1,2) << "\n"
			 << conversionMat(2,0) << " " << conversionMat(2,1) << " " << conversionMat(2,2) << "\n";

		fout.close();
	}

	void save(const std::string& fn) {
		if (!bConverted) {
			fprintf(stderr, "You have to color convert to save the converted image.\n");
			return;
		}
		if (!data) {
			fprintf(stderr, "Converted color data is empty.\n");
			return;
		}

		unsigned char* buf = new unsigned char[width*height*channels];
		for(int i=0; i<height; ++i) for(int j=0; j<width; ++j) for(int k=0; k<channels; ++k) {
			int index = channels*i*width + channels*j + k;

			float d = data[index];
			if (d <= 0.0031308) d *= 12.92;
			else				d = 1.055 * pow(d, 1 / 2.4) - 0.055;
			
			d *= 255;				
			buf[index] = static_cast<unsigned char>(d);
		}

		printf("Writing: %s :", fn.c_str());
		int success = stbi_write_png(fn.c_str(), width, height, channels, buf, width * channels);
		if (success) printf("(%d x %d x %d)\n", width, height, channels);
		else printf("Failed to save the denoised image.\n");

		delete[] buf;
	}

	static void mouseCallback(int event, int x, int y, int flags, void* param) {
		ColorConvertor* convertor = (ColorConvertor*)param;
		if (event == cv::EVENT_MOUSEMOVE) {
			if (convertor->g_bDrawingBox) {
				convertor->g_rect.width = x - convertor->g_rect.x;
				convertor->g_rect.height = y - convertor->g_rect.y;
			}
		}
		else if (event == cv::EVENT_LBUTTONDOWN)
		{
			convertor->g_bDrawingBox = true;
			convertor->g_rect = cv::Rect(x, y, 0, 0);

			printf("Point(%d, %d) RGB: %f, %f, %f\n",
				x, y,
				convertor->image.at<cv::Vec3f>(y, x)[0],
				convertor->image.at<cv::Vec3f>(y, x)[1],
				convertor->image.at<cv::Vec3f>(y, x)[2]);
		}
		else if (event == cv::EVENT_LBUTTONUP) {
			convertor->g_bDrawingBox = false;
			if (convertor->g_rect.width < 0) {
				convertor->g_rect.x += convertor->g_rect.width;
				convertor->g_rect.width *= -1;
			}
			if (convertor->g_rect.height < 0) {
				convertor->g_rect.y += convertor->g_rect.height;
				convertor->g_rect.height *= -1;
			}
			convertor->averageColor(convertor->g_rect);
			// convertor->drawRectangle(convertor->g_rect);
		}
	}

	//void selectPatch() {
	//	// win = new Window(colorchecker.width, colorchecker.height, "Color Checker");
	//	win = new Window(800, 600, "Color Checker");
	//	win->setData(buf, colorchecker.width * colorchecker.height);
	//	win->run();

	//}


	ColorConvertor() {
		
		ref << 98,  187, 166,
			   126, 125, 174,
			   82,  106, 60,
			   87,  120, 155,
			   197, 145, 125,
			   112, 76,  60,
			   222, 118, 32,
			   58,  88,  159,
			   195, 79,  95,
			   83,  58,  106,
			   157, 188, 54,
			   238, 158, 25,
			   0,   127, 159,
			   192, 75,  145,
			   245, 205, 0,
			   186, 26,  51,
			   57,  146, 64,
			   25,  55,  135,
			   249, 242, 238,
			   202, 198, 195,
			   161, 157, 154,
			   122, 118, 116,
			   80,  80,  78,
			   43,  41,  43;
	};
	virtual ~ColorConvertor() {};
};



#endif ColorConvertor_hpp