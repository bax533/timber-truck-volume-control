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
    
    void VolumeCalculator::Start(bool debug, std::string referencePath)
    {
        if(debug)
        {

            return;
        }

        //Take photo with camera and save it
        CameraHandler camHandler = CameraHandler();
        sl::Mat slImg = camHandler.GetImage();
        cv::Mat cameraImg = TimberControl::slMat2cvMat(slImg);
        imageHandler = ImageHandler(cameraImg);
        imageHandler.Prepare();

        //Try to find reference images in the photo
        TimberControl::ReferenceFinder refFinder(referencePath.c_str(), cameraImg);
        std::vector<Point> targets = refFinder.FindTargets();
        std::cout<<targets.size()<<"  size of found targets\n";
        /*if(targets.size() == 4) //Cut the photo to the area restrained by references
        {
            imageHandler.CutWithReferences(targets); 
        }*/

        imageHandler.PerformRussianMagic();
    }

    double VolumeCalculator::Calculate(cv::Mat cameraImg)
    {
        printf("%s\n", imgPath);
        
        imageHandler.PerformRussianMagic();
        return 0.0;
    }



}
