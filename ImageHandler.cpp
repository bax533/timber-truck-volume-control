#include "ImageHandler.hpp"
namespace TimberControl
{
    ImageHandler::ImageHandler(const Mat& img)
        : src(img), ksize(1), scale(1), delta(0)
    {}

    ImageHandler::ImageHandler(const char* imgPath)
        : ksize(1), scale(1), delta(0)
    {
        src = imread(imgPath);
    }

    ImageHandler::ImageHandler(const char* imgPath, int ksize, int scale, int delta)
        : ksize(ksize), scale(scale), delta(delta)
    {
        src = imread(imgPath, IMREAD_COLOR);
    }

    void ImageHandler::Prepare()
    {
        std::cout<<"a\n";
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        std::cout<<"b\n";
        //cvtColor(src, src_gray, COLOR_BGR2GRAY);
        src_gray = src;
        std::cout<<"c\n";
        int ddepth = CV_16S;

        Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
         std::cout<<"d\n";    
        Mat grad_x_abs, grad_y_abs;

        convertScaleAbs(grad_x, grad_x_abs);
        convertScaleAbs(grad_y, grad_y_abs);
                std::cout<<"e\n";
        grad_x = grad_x_abs;
        grad_y = grad_y_abs;

        addWeighted(grad_x_abs, 0.5, grad_y_abs, 0.5, 0, grad_xy);
        
        Canny(src_gray, grad_xy_thin, cannyLowThresh, cannyHighThresh, 3, false);
                std::cout<<"f:\n"; 
        R.create(grad_x.rows, grad_x.cols, CV_8U);
        P.create(grad_x.rows, grad_x.cols, CV_8U);
    }

    void ImageHandler::PerformRussianMagic()
    {
        uchar* image_data = src_gray.data;
        uchar* grad_x_data = grad_x.data;
        uchar* grad_y_data = grad_y.data;
        uchar* thin_data = grad_xy_thin.data;

        int height = grad_x.rows;
        int width = grad_x.cols;
        int step = grad_x.step;

        for(int row = 0; row < height; row++)
        {   
            /*
            uchar* thin_p = grad_xy_thin.ptr(row);
            uchar* grad_x_p = grad_x.ptr(row);
            uchar* grad_y_p = grad_y.ptr(row);
            uchar* grey_p = src_gray.ptr(row);
            */
            for(int col = 0; col < width; col++)
            {
                std::pair<int, int> center = {row, col};
                FindBestCircle(center);
            }
        } 
    }   

    void ImageHandler::FindBestCircle(std::pair<int, int> center)
    {
        int bestR = -1;

        int height = grad_x.rows;
        int width = grad_x.cols;

        Mat angleOk(Size(2*maxR, 2*maxR), CV_8U, Scalar(0));
        for(int row = center.first - maxR; row <= center.first + maxR/2; row++)
            for(int col = center.second - maxR; col <= center.second + maxR/2; col++)
            {
                if(!InBounds({row, col}, width, height))
                    continue;
                Vector<int> toCenter = {center.second-col, center.first-row}; //x, y
                Vector<int> gradAngle = {(int)grad_x.at<uchar>(row, col), (int)grad_y.at<uchar>(row, col)};
                
                float toCenter_th = atan2(toCenter.y, toCenter.x)*180/PI; 
                float grad_th = 0;
                if(gradAngle.x != 0 && gradAngle.y != 0)
                    grad_th = atan2(gradAngle.y, gradAngle.x) * 180 / PI;

                if(abs(AngleBetweenVectors(toCenter, gradAngle)) < 1)
                    angleOk.at<uchar>(row - center.first + maxR, col - center.second + maxR) = 255;
            }
        imshow("dupa", angleOk);
        waitKey(0); 
    }

    void ImageHandler::showImage(imgNum img)
    {
        printf("grey rows cols %d %d\n_grad_x rows cols %d %d\n", src_gray.rows, src_gray.cols, grad_x.rows, grad_x.cols);
        switch(img)
        {
            case n_src:
                imshow (windowName,src);
                waitKey(0);
                break;

            case n_grad_x:
                imshow(windowName, grad_x);
                waitKey(0);
                break;

            case n_grad_y:
                imshow(windowName, grad_y);
                waitKey(0);
                break;

            case n_grad_xy:
                imshow (windowName, grad_xy);
                waitKey(0);
                break;

            case n_grad_xy_thin:
                imshow(windowName, grad_xy_thin);
                waitKey(0);
                break;

            default:
                break;
        }
    }
}       
