#ifndef Denoiser_hpp
#define Denoiser_hpp


#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <string>
#include <vector>
#include <algorithm>


class Denoiser {
public:
	float* data = nullptr;
	int width=0, height=0, channels=3;
	bool bDenoised = false;


	void gaussianDenoise(int size, float sigma);
	void medianDenoise(int size);
	void bilateralDenoise(int size, float sigmaSpatial, float sigmaRange);


	void save(const std::string& fn){
		if (!bDenoised) {
			fprintf(stderr, "You have to denoise to save the denoied image.\n");
			return;
		}
		if (!data) {
			fprintf(stderr, "Denoised data is empty.\n");
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
	}

	
	Denoiser() {};
	~Denoiser() {};

};


#endif Denoiser_hpp