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
    volumeCalculator.Start(reference_image_path, 8.4, 1.4);
}
