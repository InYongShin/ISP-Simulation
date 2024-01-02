
#include "Denoiser.hpp"

const float pi = 3.141592f;


static inline float Gauss(int x, int y, float sigma2) {
	float a = 1 / (2*pi*sigma2);
    float b = exp( - ((x*x)+(y*y)) / (2*sigma2));
    return a * b;
}

static inline float Gauss(float k, float sigma2) {
	float a = 1 / sqrt(2*pi*sigma2);
    float b = exp( - (k*k)/(2*sigma2));
    return a * b;
}


void Denoiser::gaussianDenoise(int size, float sigma) {
    if(size<1){
        fprintf(stderr, "Kernel size must be one or more.\n");
        return;
    }
    if(size%2==0){
        printf("Kernel size cannnot even number. Size will be changed to %d.\n", size-1);
        size -= 1;
    }

    float* kernel = new float[size*size];
    float sum = 0.f;
    float sigma2 = sigma*sigma;

    for (int y=0; y<size; y++) for(int x=0; x<size; ++x) {
        kernel[y*size + x] = Gauss(x-(size/2), y-(size/2), sigma2);
        sum += kernel[y*size + x];
    }
    for (int y=0; y<size; y++) for(int x=0; x<size; ++x) {
        kernel[y*size + x] /= sum;
    }


    float* copyData = new float[3*width* height];
    memcpy(copyData, data, 3*width*height*sizeof(float));

    for(int y=size/2; y<height-(size/2); ++y) for(int x=size/2; x<width-(size/2); ++x){
        float r = 0.f;
        float g = 0.f;
        float b = 0.f;

        for(int ky=0; ky<size; ++ky) for(int kx=0; kx<size; ++kx){
            r += copyData[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2))    ] * kernel[ky*size + kx];
            g += copyData[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 1] * kernel[ky*size + kx];
            b += copyData[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 2] * kernel[ky*size + kx];
        }

        data[3*(y*width + x)    ] = r;
        data[3*(y*width + x) + 1] = g;
        data[3*(y*width + x) + 2] = b;
    }

    bDenoised = true;
    delete[] kernel;
    delete[] copyData;
}

void Denoiser::medianDenoise(int size) {
    if(size<1){
        fprintf(stderr, "Kernel size must be one or more.\n");
        return;
    }
    if(size%2==0){
        printf("Kernel size cannnot even number. Size will be changed to %d.\n", size-1);
        size -= 1;
    }

    float* copyBuf = new float[3*width*height];
    memcpy(copyBuf, data, 3*width*height*sizeof(float));

    for(int y=size/2; y<height-(size/2); ++y) for(int x=size/2; x<width-(size/2); ++x){
        std::vector<float> r(size*size);
        std::vector<float> g(size*size);
        std::vector<float> b(size*size);

        for(int ky=0; ky<size; ++ky) for(int kx=0; kx<size; ++kx){
            r[ky*size + kx] = copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2))    ];
            g[ky*size + kx] = copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 1];
            b[ky*size + kx] = copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 2];
        }

        std::sort(r.begin(), r.end());
        std::sort(g.begin(), g.end());
        std::sort(b.begin(), b.end());

        data[3*y*width + 3*x    ] = r[(size*size)/2];
        data[3*y*width + 3*x + 1] = g[(size*size)/2];
        data[3*y*width + 3*x + 2] = b[(size*size)/2];
    }
    
    bDenoised = true;
    delete[] copyBuf;
}

void Denoiser::bilateralDenoise(int size, float sigmaSpatial, float sigmaRange){
    if(size<1){
        fprintf(stderr, "Kernel size must be one or more.\n");
        return;
    }
    if(size%2==0){
        printf("Kernel size cannnot even number. Size will be changed to %d.\n", size-1);
        size -= 1;
    }

    float* copyBuf = new float[3*width* height];
    memcpy(copyBuf, data, 3*width*height*sizeof(float));

    float* rangeKernel = new float[size*size];
    float* spatialKernel = new float[size*size];
    float sigmaRange2 = sigmaRange * sigmaRange;
    float sigmaSpatial2 = sigmaSpatial * sigmaSpatial;
    float sum = 0.f;

    for(int y=size/2; y<height-(size/2); ++y) for(int x=size/2; x<width-(size/2); ++x){
        float r = 0.f;
        float g = 0.f;
        float b = 0.f;
        sum = 0.f;
        
        for(int ky=0; ky<size; ++ky) for(int kx=0; kx<size; ++kx){
            float bright_filter = 0.2126 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2))    ] +
                                  0.7152 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 1] +
                                  0.0722 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 2];
            float bright_image = 0.2126 * copyBuf[3*y*width + 3*x    ] +
                                 0.7152 * copyBuf[3*y*width + 3*x + 1] +
                                 0.0722 * copyBuf[3*y*width + 3*x + 2];
            float k = (255*bright_filter - 255*bright_image) * (255*bright_filter - 255*bright_image);

            // printf("%f %f %f\n", bright_filter, bright_image, k);

            float range = Gauss(k, sigmaRange2);
            // printf("%f\n", range);
            float spatial = Gauss(kx-(size/2), ky-(size/2), sigmaSpatial2);

            sum += range * spatial;
        }

        for(int ky=0; ky<size; ++ky) for(int kx=0; kx<size; ++kx){

            float bright_filter = 0.2126 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2))    ] +
                                  0.7152 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 1] +
                                  0.0722 * copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 2];
            float bright_image = 0.2126 * copyBuf[3*y*width + 3*x    ] +
                                 0.7152 * copyBuf[3*y*width + 3*x + 1] +
                                 0.0722 * copyBuf[3*y*width + 3*x + 2];
            float k = (255*bright_filter - 255*bright_image) * (255*bright_filter - 255*bright_image);

            float range = Gauss(k, sigmaRange2);
            float spatial = Gauss(kx-(size/2), ky-(size/2), sigmaSpatial2);

            r += (copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2))    ] * range * spatial) / sum;
            g += (copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 1] * range * spatial) / sum;
            b += (copyBuf[3*(y-(ky-size/2))*width + 3*(x-(kx-size/2)) + 2] * range * spatial) / sum;
        }

        data[3*(y*width + x)    ] = r;
        data[3*(y*width + x) + 1] = g;
        data[3*(y*width + x) + 2] = b;
    }

    bDenoised = true;
    delete[] copyBuf;
}
