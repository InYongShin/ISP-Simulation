#ifndef Image_hpp
#define Image_hpp


#include "StringUtil.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>


class Image {
public:
	int width=0;
	int height=0;
	int channels=0;
	std::string path="";
	std::string name="";
	std::string extension="";
	float* data = nullptr;
	bool bDemosaiced = false;

	cv::Mat image = cv::Mat(0,0,CV_16UC1);

	bool load(const std::string& fileName, int blackLevel=0, int bit=16);
	


	bool isEmpty() const {
		return (data==nullptr);
	}
	
	void clear() {
		width = 0; height = 0; channels=0;
		path = ""; name = ""; extension = "";
		if(data) free(data);
		data = nullptr;
	}

	void print() {
		std::cout << "====================" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height  << std::endl;
		std::cout << "Channels: " << channels << std::endl;
		std::cout << "File path: " << path << std::endl;
		std::cout << "File name: " << name << std::endl;
		std::cout << "Extension: " << extension << std::endl;
		std::cout << "====================" << std::endl;
	}



	Image() {};
	virtual ~Image() { clear(); };
};


#endif FileLoader_hpp