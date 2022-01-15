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

        void Start(std::string referencePath, double camToTrailerBegin_dist, double distBetweenTargets_vert);
         
        double Calculate(const std::vector<Circle>& circles, double pxMultiplier);
        void FindParameters();
    };
}

#endif
