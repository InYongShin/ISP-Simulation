#ifndef Meterer_hpp
#define Meterer_hpp


#include <stb/stb_image_write.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>



class Meterer {
public:
	float* data = nullptr;
	int width=0, height=0, channels=3;
	bool bMetered = false;
	int moustPtX=0, moustPtY=0;
	
	void spotMeter(int x, int y);
	void centerWeightedMeter();


	void save(const std::string& fn) {
		if (!bMetered) {
			fprintf(stderr, "You have to meter to save the metered image.\n");
			return;
		}
		if (!data) {
			fprintf(stderr, "Metered data is empty.\n");
			return;
		}
		unsigned char* buf = new unsigned char[width*height*channels];
		for (int i=0; i<height; ++i) for (int j=0; j<width; ++j) for (int k=0; k<channels; ++k) {
			int index = channels*i*width + channels*j + k;
			float d = data[index];

			d = pow(d, 1 / 2.2f);
			// if (d <= 0.0031308) d *= 12.92;
			// else				d = 1.055 * pow(d, 1/2.4) - 0.055;

			d *= 255;				
			buf[index] = static_cast<unsigned char>(d);
		}

		printf("Writing: %s :", fn.c_str());
		int success = stbi_write_png(fn.c_str(), width, height, channels, buf, width * channels);
		if (success) printf("(%d x %d x %d)\n", width, height, channels);
		else printf("Failed to save the denoised image.\n");
	}



	static void mouseCallback(int event, int x, int y, int flags, void* param) {
		Meterer* meterer = (Meterer*)param;

		if (event == cv::EVENT_LBUTTONUP)
		{
			printf("Point(%d, %d) RGB: %f, %f, %f\n",
				   x, y,
				   meterer->data[y * meterer->width + x],
				   meterer->data[y * meterer->width + x + 1],
				   meterer->data[y * meterer->width + x + 2]);

			meterer->moustPtX = x;
			meterer->moustPtY = y;
			cv::destroyWindow("Metering");
		}
	}


};

#endif Meterer_hpp