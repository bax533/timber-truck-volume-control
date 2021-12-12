#include "VolumeCalculator.hpp"
#include "Tester.hpp"
#include "ReferenceFinder.hpp"

using namespace cv;

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("usage: ./VolumeCalculator <wood-img-path> <reference-template-path>\n");
        return 0;
    }    
    const char *path = argv[1];
    
    TimberControl::ReferenceFinder refFinder(argv[2], argv[1]);
    std::vector<Point> targets = refFinder.FindTargets();
   
    return 0; 
    TimberControl::VolumeCalculator volumeCalc(path);
    volumeCalc.Calculate();

    /*
    TimberControl::Tester tester = TimberControl::Tester();
    cv::Mat img = imread(path, IMREAD_COLOR);
    cv::cvtColor(img, img, COLOR_BGR2GRAY);

    tester.test(img);    
    */
}
