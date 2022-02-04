#ifndef TESTER_H
#define TESTER_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <iostream>
#include "ImageHandler.hpp"

#include "math.h"


using namespace std;
using namespace cv;

namespace TimberControl
{
class Tester
{
public:
    Tester()
    {

    }

    void test(Mat& img)
    {
        Mat testImg(Size(100, 100), CV_8U, Scalar(0));
        
        circle(testImg, Point(49, 49), 20, Scalar(255));
        ImageHandler imgHandler(testImg);
        imgHandler.Prepare();
        
        imgHandler.FindBestCircle({49, 49});
    }

    void showImg(cv::Mat img)
    {
        imshow("test_window", img);
        waitKey(0);
    }
};
}
#endif
