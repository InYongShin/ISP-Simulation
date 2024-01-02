#ifndef Simulator_hpp
#define Simulator_hpp


#include "Image.hpp"
#include "Demosaicer.hpp"
#include "Denoiser.hpp"
#include "ColorConvertor.hpp"
#include "Meterer.hpp"


enum class eDemosaicing {
	RGGB_Linear,
	GBRG_Linear
};

enum class eDenoising {
	Gaussian,
	Median,
	Bilateral
};

enum class eMetering {
	Spot,
	Center
};


class Simulator {
private:
public:
	eDemosaicing demosaicingMode = eDemosaicing::RGGB_Linear;
	eDenoising denosingMode = eDenoising::Gaussian;
	eMetering meteringMode = eMetering::Spot;
	Demosaicer demosaicer;
	ColorConvertor colorConvertor;
	Denoiser denoiser;
	Meterer meterer;
	Image image;

	bool load(const std::string& fileName, int blackLevel, int bit) {
		image.clear();
		return image.load(fileName, blackLevel, bit);
	}

	void demosaic(eDemosaicing mode) {
		printf("Demosaicing ...\n");
		if (!image.data) {
			fprintf(stderr, "Image data is empty.\n");
			return;
		}

		demosaicer.data = image.data;
		demosaicer.width = image.width;
		demosaicer.height = image.height;
		demosaicer.channels = image.channels;

		demosaicingMode = mode;
		switch (mode)
		{
		case eDemosaicing::RGGB_Linear:
			demosaicer.linearDemosaicRGGB();
			break;
		case eDemosaicing::GBRG_Linear:
			demosaicer.linearDemosaicGBRG();
			break;
		default:
			break;
		}
		image.data = demosaicer.data;
		demosaicer.channels = 3;
		image.channels = 3;
	}

	void saveDemosaicedImg(const std::string& fn) {
		demosaicer.save(fn);
	}

	void denoise(eDenoising mode, int kernelSize=1, float sigma=1.f, float sigmaRange=1.f) {
		printf("Denoising ...\n");
		if (!image.data) {
			fprintf(stderr, "Demosaiced data is empty.\n");
			return;
		}
		denoiser.data = image.data;
		denoiser.width = image.width;
		denoiser.height = image.height;
		denoiser.channels = image.channels;

		denosingMode = mode;
		switch (denosingMode) {
		case eDenoising::Gaussian:
			denoiser.gaussianDenoise(kernelSize, sigma);
			break;
		case eDenoising::Median:
			denoiser.medianDenoise(kernelSize);
			break;
		case eDenoising::Bilateral:
			denoiser.bilateralDenoise(kernelSize, sigma, sigmaRange);
			break;
		default:
			break;
		}
		image.data = denoiser.data;
	}

	void saveDenoisedImg(const std::string& fn){
		denoiser.save(fn);
	}

	void camRGB2SRGB(const std::string& fn = "") {
		colorConvertor.data = image.data;
		colorConvertor.width = image.width;
		colorConvertor.height = image.height;
		colorConvertor.channels = image.channels;
		if (fn.size() > 0) {
			colorConvertor.getConversionMat(fn);
		}
		else {
			colorConvertor.selectPatch();
			colorConvertor.computeConversionMat();
		}
		colorConvertor.colorConvert();
		image.data = colorConvertor.data;
	}

	void saveConversionMat(const std::string& fn) {
		colorConvertor.saveConversionMat(fn);
	}

	void saveSRGB(const std::string& fn) {
		colorConvertor.save(fn);
	}

	void meter(eMetering mode, int x=10, int y=10) {
		printf("Metering ...\n");
		if (!image.data) {
			fprintf(stderr, "Image data is empty.\n");
			return;
		}
		meterer.data = image.data;
		meterer.width = image.width;
		meterer.height = image.height;
		meterer.channels = image.channels;

		meteringMode = mode;
		switch(meteringMode){
		case eMetering::Spot:
			meterer.spotMeter(x, y);
			break;
		case eMetering::Center:
			meterer.centerWeightedMeter();
			break;
		default:
			break;
		}
		image.data = meterer.data;
	}

	void saveMeteredImg(const std::string& fn) {
		meterer.save(fn);
	}

	void print() {
		if (image.isEmpty()) {
			std::cout << "You have not any image." << std::endl;
			return;
		}
		image.print();
	}

	Simulator() {};
	~Simulator() {};

};




#endif Simulator_hpp