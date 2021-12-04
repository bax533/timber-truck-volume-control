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
        /*
        GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
        Canny(img, img, 100, 200, 3, false);

        double minVal, maxVal;
        minMaxLoc(img, &minVal, &maxVal);
       
        cout<<minVal<< " "<<maxVal<<"  min max\n";
        //normalize(img, img, 0, 255, NORM_MINMAX); 

        for(int j=0; j<img.rows; j++)
        {
            for(int i = 0; i < img.cols; i++)
            {   
                //cout<<(int)img.at<uchar>(j, i)<<" ";
                if(img.at<uchar>(j, i) >= 250)dd
                   // floodFill(img, Point(i, j), (0,0,0));
            }
            cout<<"\n";
        }
        
        imshow("ou", img);
        waitKey(0);
        return;
        */

        Mat testImg(Size(100, 100), CV_8U, Scalar(0));
        
        circle(testImg, Point(49, 49), 20, Scalar(255));
        ImageHandler imgHandler(testImg);
        imgHandler.Prepare();
        //for(int i=48; i<=50; i++)
        //    for(int j =49; j<=50; j++)
              imgHandler.FindBestCircle({49, 49});
        //imshow("dupa", testImg);
        waitKey(0);
    }

    void showImg(cv::Mat img)
    {
        imshow("test_window", img);
        waitKey(0);
    }
};
}
#endif
