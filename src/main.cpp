#include "VolumeCalculator.hpp"
#include "Tester.hpp"

#include <opencv2/opencv.hpp>
// OpenCV dep
#include <opencv2/cvconfig.h>

using namespace sl;
using namespace cv;

std::map<std::string, bool> input_flags;
std::string input_image_path = "";
std::string reference_image_path = "";

int main(int argc, char* argv[])
{
    
    //cv::imshow("cameraImage",cameraImg);
    //waitKey(0);  

    for(int i = 0; i < argc; i++)
    {
        if(std::strcmp(argv[i], "--find-reference") == 0)
        {
            input_flags["find-reference"] = true;
            reference_image_path = argv[i+1]; 
        }

        if(std::strcmp(argv[i], "--input-image") == 0)
        {
            input_flags["input-image"] = true;
            input_image_path = argv[i+1]; 
        }
    }

    TimberControl::VolumeCalculator volumeCalculator;
    volumeCalculator.Start(reference_image_path, 2.4, 2.0);

    /*
    TimberControl::Tester tester = TimberControl::Tester();
    cv::Mat img = imread(path, IMREAD_COLOR);
    cv::cvtColor(img, img, COLOR_BGR2GRAY);

    tester.test(img);    
    */
}
