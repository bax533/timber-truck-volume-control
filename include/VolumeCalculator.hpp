#ifndef VOLUME_CALCULATOR_H
#define VOLUME_CALCULATOR_H

#include "ImageHandler.hpp"
#include "CameraHandler.hpp"
#include "ReferenceFinder.hpp"

namespace TimberControl
{
    class VolumeCalculator
    {
        const char *imgPath;
        ImageHandler imageHandler;

    public:

        VolumeCalculator() = default;
        VolumeCalculator(cv::Mat image);
        VolumeCalculator(const char* imgPath);

        void Start(bool debug, std::string referencePath);
         
        double Calculate(cv::Mat cameraImg);
  };
}

#endif
