#ifndef REFERENCE_FINDER_H
#define REFERENCE_FINDER_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>

#include "Profiler.hpp"
#include "Common.hpp"

namespace TimberControl
{

    //using namespace cv;

class ReferenceFinder
{

private:
    Mat reference, source;

public:
    
    ReferenceFinder() = default;

    ReferenceFinder(const char *refPath, const char *imgPath)
    {
        reference = imread(refPath, IMREAD_COLOR);
        source = imread(imgPath, IMREAD_COLOR);

        cvtColor(reference, reference, COLOR_BGR2GRAY);
        cvtColor(source, source, COLOR_BGR2GRAY);
         
    }

    ReferenceFinder(const char *refPath, const cv::Mat& cameraImage)
        : source(cameraImage)
    {
        reference = imread(refPath, IMREAD_COLOR);

        cvtColor(reference, reference, COLOR_BGR2GRAY);
        cvtColor(source, source, COLOR_BGR2GRAY);
    }
    Area FindTargets();
};

}

#endif
