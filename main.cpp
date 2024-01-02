#include <iostream>

#include "Simulator.hpp"



int main()
{
    Simulator* simulator = new Simulator();

    bool success = simulator->load("BayerImages/galaxy_things2/DngRaw2_2.png", 0, 12);
    if (!success) exit(1);
    simulator->print();


    /* Demosaicing */
    simulator->demosaic(eDemosaicing::GBRG_Linear);
    simulator->saveDemosaicedImg("DemosaicedImages/galaxy_things2/DngRaw2_2.png");
    

    /* Denoising */
    simulator->denoise(eDenoising::Gaussian, 5, 1);
    simulator->saveDenoisedImg("DenoisedImages/galaxy_things2/DngRaw2_2_Linear_Gaussian_5_1.png");
    
    
    /* Color Conversion */
    // simulator->camRGB2SRGB();
    // simulator->saveConversionMat("Results/galaxy_color_conversion_matrix_indoor_ISO3200.txt");
    simulator->camRGB2SRGB("Results/galaxy_color_conversion_matrix_indoor_ISO320.txt");
    simulator->saveSRGB("Results/galaxy_things2/DngRaw2_2_Linear_Gaussian_5_1.png");


    /* Metering */
    simulator->meter(eMetering::Spot, 50, 50);
    simulator->saveMeteredImg("MeteredImages/galaxy_things2/DngRaw2_2_Linear_Gaussian_5_1_Spot.png");





    /* OpenCV */

    //cv::Mat img = cv::imread("DemosaicedImages/sony_colorchecker/SonyRaw_0.png", -1);
    // cv::Mat img = cv::imread("test_demosaiced_8bit.png", -1);
    //// cv::medianBlur(img, img, 5);
    // cv::GaussianBlur(img, img, cv::Size(9,9), 5, 5);
    //// cv::Mat biImg;
    //// cv::bilateralFilter(img, biImg, 5, 2., 10.);
    // cv::imwrite("test_denoised_8bit.png", img);


    return 0;
}