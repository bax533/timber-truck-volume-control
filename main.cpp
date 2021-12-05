#include "VolumeCalculator.hpp"
#include "Tester.hpp"

using namespace cv;

int main(int argc, char* argv[])
{
    const char *path = argv[1];
    TimberControl::VolumeCalculator volumeCalc(path);
    volumeCalc.Calculate();

    /*
    TimberControl::Tester tester = TimberControl::Tester();
    cv::Mat img = imread(path, IMREAD_COLOR);
    cv::cvtColor(img, img, COLOR_BGR2GRAY);

    tester.test(img);    
    */
}
