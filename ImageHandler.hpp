#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

#include "Common.hpp"

namespace TimberControl
{
    //using namespace cv;
    enum imgNum
    {
        n_src,
        n_grey,
        n_grad_x,
        n_grad_y,
        n_grad_xy,
        n_grad_xy_thin,
        n_phase
    };

    class ImageHandler
    {
        Mat src, src_gray, grad_x, grad_y, grad_xy, grad_xy_thin, phase_img;
        Mat R, P;
        int ksize, scale, delta;
        int cannyLowThresh = 100, cannyHighThresh = 200;

        //int rMin = 5, rMax = 75; 
            
    public:
        ImageHandler() = default;
        ImageHandler(const Mat& img);
        ImageHandler(const char* imgPath);
        ImageHandler(const char* imgPath, int ksize, int scale, int delta);
   

        void Prepare(); 
        void showImage(imgNum img); 

        void PerformRussianMagic();
        void FindBestCircle(std::pair<int, int> center);
    
    };
}

#endif
