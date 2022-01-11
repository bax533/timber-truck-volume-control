#include "VolumeCalculator.hpp"

#include <cstdio>

namespace TimberControl
{

    VolumeCalculator::VolumeCalculator(cv::Mat image)
    {
        imageHandler = ImageHandler(image);
        imageHandler.Prepare();
    }

    VolumeCalculator::VolumeCalculator(const char* imgPath)
        : imgPath(imgPath)
    {
        imageHandler = ImageHandler(imgPath);
        imageHandler.Prepare();
    }
   
    //distance in [m] 
    void VolumeCalculator::Start(std::string referencePath, double camToTrailerBegin_dist, double distBetweenTargets_vert)
    {
        //Take photo with camera and save it
        CameraHandler camHandler = CameraHandler();

        //sl::Mat slImg = camHandler.GetImage();
        //cv::Mat cameraImg = TimberControl::slMat2cvMat(slImg);
        
        cv::Mat cameraImg = imread("wood_with_new_template.png");
        
        imageHandler = ImageHandler(cameraImg);
        imageHandler.Prepare();

        //Try to find reference images in the photo
        TimberControl::ReferenceFinder refFinder(referencePath.c_str(), cameraImg);
        Area searchArea = refFinder.FindTargets();
        std::vector<Circle> foundCircles = imageHandler.PerformRussianMagic(searchArea);
        std::cout<<foundCircles.size()<<" "<<"?\n";
        double px_to_metres = distBetweenTargets_vert/(double)(searchArea.lower_l.y - searchArea.upper_l.y);
        
        double volume = 0.0;

        double D = camToTrailerBegin_dist;
        for(const Circle& circle : foundCircles)
        {
            double circleArea = (double)circle.r * px_to_metres;

            double distToAbut = camHandler.GetDistance(circle.center.x, circle.center.y);
            double alpha = horizontalAngleForPixel(circle.center.x, cameraImg.cols, cameraImg.rows);
            double l = D - distToAbut * std::sin(1.5708 - alpha); 

            volume += circleArea * l;
        }
        
        std::cout<<volume<<" sum of area\n";
    }

    double VolumeCalculator::Calculate(const std::vector<Circle>& circles, double px_to_metres)
    {
        double volume = 0.0;
        for(const Circle& circle : circles)
        {
            double circleArea = circle.r * px_to_metres;
            volume += circleArea; 
        }
        
       return volume;
    }

}
