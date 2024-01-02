
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


bool Image::load(const std::string& fileName, int blackLevel, int bit) {
	path = backToFrontSlash(fileName);
	name = getName(fileName);
	extension = getExtension(fileName);

	std::cout << "Loading: " << name << " ";
	uint16_t* buf = (uint16_t*)stbi_load_16(fileName.c_str(), &width, &height, &channels, 0);
	if(!buf){
		std::cout << "Failed to load the bayer image: " << name << std::endl;
		return false;
	}
	printf("(%d x %d x %d): %s\n", width, height, channels, fileName.c_str());

	if (data) free(data);
	data = nullptr;

	int normalize = static_cast<int>(pow(2,bit)-1);

	printf("Black Level: %d\n", blackLevel);
	printf("Converting %dbit data to float (0~1) ...\n", bit);
	data = new float[width * height * channels];
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			uint16_t b = buf[i*width + j];
			if( b < blackLevel ) b = 0;
			else				 b -= blackLevel;

			data[i*width + j] = float(b) / normalize;
			float d = data[i*width + j];
			if(d<0) data[i*width + j] = 0;
			if(d>1) data[i*width + j] = 1;
		}
	}
	free(buf);
	return true;
}
