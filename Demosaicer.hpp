#ifndef Demosaicer_hpp
#define Demosaicer_hpp


#include <stb/stb_image_write.h>

#include <iostream>


class Demosaicer {
public:
	float* data = nullptr;
	int width=0, height=0, channels=3;
	bool bDemosaiced = false;

	void setData(float* data) {
		this->data = data;
	}

	void linearDemosaicRGGB();
	void linearDemosaicGBRG();


	void save(const std::string& fn) {
		if (!bDemosaiced) {
			fprintf(stderr, "You have to demosaic to save the denoied image.\n");
			return;
		}
		if (!data) {
			fprintf(stderr, "Demosaiced data is empty.\n");
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
		else printf("Failed to save the demosaiced image.\n");
	}


};


#endif Demosaicer_hpp