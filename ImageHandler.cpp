#include "ImageHandler.hpp"
namespace TimberControl
{
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
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        int ddepth = CV_16S;

        Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);


            
        Mat grad_x_abs, grad_y_abs;

        convertScaleAbs(grad_x, grad_x_abs);
        convertScaleAbs(grad_y, grad_y_abs);

        grad_x = grad_x_abs;
        grad_y = grad_y_abs;

        addWeighted(grad_x_abs, 0.5, grad_y_abs, 0.5, 0, grad_xy);
        


    }

    void ImageHandler::showImage(imgNum img)
    {
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

            default:
                break;
        }
    }
}       
