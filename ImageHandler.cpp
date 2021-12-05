#include "ImageHandler.hpp"
namespace TimberControl
{
    ImageHandler::ImageHandler(const Mat& img)
        : src(img), ksize(1), scale(1), delta(0)
    {
        R = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0)); 
        N = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0));
    }

    ImageHandler::ImageHandler(const char* imgPath)
        : ksize(1), scale(1), delta(0)
    {
        src = imread(imgPath);
        R = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0)); 
        N = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0));
    }

    ImageHandler::ImageHandler(const char* imgPath, int ksize, int scale, int delta)
        : ksize(ksize), scale(scale), delta(delta)
    {
        src = imread(imgPath, IMREAD_COLOR);
        R = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0)); 
        N = Mat(Size(src.rows, src.cols), CV_32SC1, Scalar(0));
    }

    void ImageHandler::Prepare()
    {
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        //src_gray = src;
        int ddepth = CV_32F;

        Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
        Mat grad_x_abs, grad_y_abs;

        phase(grad_x, grad_y, phase_img, true);
 
        //std::cout<<phase_img.rows<<", "<<phase_img.cols<<" phase rows, cols\n";
        //std::cout<<src_gray.rows<<", "<<src_gray.cols<<" src rows, cols\n";

        convertScaleAbs(grad_x, grad_x_abs);
        convertScaleAbs(grad_y, grad_y_abs);
        //grad_x = grad_x_abs;
        //grad_y = grad_y_abs;
        
        addWeighted(grad_x_abs, 0.5, grad_y_abs, 0.5, 0, grad_xy);
        
        Canny(src_gray, grad_xy_thin, cannyLowThresh, cannyHighThresh, 3, false);
    }

    void ImageHandler::PerformRussianMagic()
    {
        int height = grad_x.rows;
        int width = grad_x.cols;
        //int step = grad_x.step;

        for(int row = 0; row < height; row++)
        {   
            for(int col = 0; col < width; col++)
            {
                FindBestCircle(Point(row, col));
            }
            std::cout<<(double)row/(double)height * 100.0<<"\n";
        }

        Mat resultImg = Mat::zeros(src_gray.rows, src_gray.cols, CV_8U);
        int maxVal=-1;
        Point maxPoint;
        getMaxAndPos(N, maxVal, maxPoint);
        int curR = (int)R.at<uchar>(maxPoint.y, maxPoint.x);
        
        std::cout<<maxVal<<"  <- maxVal  cur R -> "<<(int)curR<<"\n";
        while((double)maxVal/(double)curR > 2.0)
        {
            //if(curR == 0)
            //{
                
            //}
            circle(resultImg, maxPoint, curR, Scalar(255), FILLED);
            circle(R, maxPoint, curR, Scalar(0), FILLED);
            getMaxAndPos(N, maxVal, maxPoint);
            curR = (int)R.at<uchar>(maxPoint.x, maxPoint.y);
        }
        imshow("kolkas", resultImg);
        waitKey(0);
    }   

    void ImageHandler::FindBestCircle(Point center)
    {
        int height = grad_x.rows;
        int width = grad_x.cols;

        int rows_from = center.x - maxR < 0 ? 0 : center.x - maxR;
        int rows_to = center.x + maxR >= height ? height : center.x + maxR;
        int cols_from = center.y - maxR < 0 ? 0 : center.y - maxR;
        int cols_to = center.y + maxR >= width ? width : center.y + maxR;

        if(center.x - maxR < 0) return;
        if(center.x + maxR >= height) return;
        if(center.y - maxR < 0) return;
        if(center.y + maxR >= width) return;

        Mat toCenter_mat(Size(2*maxR, 2*maxR), CV_32F, Scalar(0));
        Mat current_phase = phase_img(
                Range(rows_from, rows_to),
                Range(cols_from, cols_to));
        
        Mat current_canny = grad_xy_thin(
                Range(rows_from, rows_to),
                Range(cols_from, cols_to));
        
        //std::cout<<center.x<<", "<<center.y<<"\n";
        center = Point(center.x - rows_from, center.y - cols_from);
        
        for(int row=0; row<2*maxR; row++)
            for(int col = 0; col < 2*maxR; col++)
            {
                Vector<int> toCenter = {center.y - col, center.x - row};
                float toCenter_th = atan2(toCenter.y, toCenter.x) * 180/PI;
                toCenter_mat.at<float>(row, col) = toCenter_th >= 0 ? toCenter_th : 360 + toCenter_th; 
            }

        
        //std::cout<<toCenter_mat.rows<<", "<<toCenter_mat.cols<<" toCenter rows, cols\n";
        //std::cout<<current_canny.rows<<", "<<current_canny.cols<<" phase rows, cols\n";

        Mat result = abs(toCenter_mat - current_phase);
        
        threshold(result, result, 10, 255, 1);
        result.convertTo(result, CV_8U);
        current_canny.convertTo(current_canny, CV_8U);
        
        //std::cout<<result.rows<<", "<<result.cols<<" result rows, cols\n";
        //std::cout<<current_canny.rows<<", "<<current_canny.cols<<" canny rows, cols\n";
        
        bitwise_and(result, current_canny, result);
        
        int bestN = -1;
        int bestR = -1;

        for(int r = maxR; r >= minR; r--)
        {
            Mat circleMat = Mat::zeros(2*maxR, 2*maxR, CV_8U);
            circle(circleMat, Point(center.x, center.y), r, Scalar(255));
            bitwise_and(result, circleMat, circleMat);
            int n = countNonZero(circleMat); 
            if(n > bestN)
            {
                bestN = n;
                bestR = r;
            }
        }

        //Mat circleMat(Size(2*maxR, 2*maxR), CV_8U, Scalar(0));
        //circle(circleMat, Point(center.x, center.y), bestR, Scalar(255));

        //std::cout<<bestR<<": bestR, bestN:"<<bestN<<"\n";
       

        R.at<int>(center.x, center.y) = bestR;
        N.at<int>(center.x, center.y) = bestN; 

        //imshow("dupa dupa dupa",circleMat);
        //waitKey(0);
        /*
        for(int row = center.x - maxR; row <= center.x + maxR; row++)
        {
            for(int col = center.y - maxR; col <= center.y + maxR; col++)
            {
                if(!InBounds({row, col}, width, height))
                    continue;
                Vector<int> toCenter = {center.y-col, center.x-row}; //x, y
                Vector<int> gradAngle = {(int)grad_x.at<uchar>(row, col), (int)grad_y.at<uchar>(row, col)};
                
                float toCenter_th = atan2(toCenter.y, toCenter.x)*180/PI; 
                float grad_th = phase_img.at<float>(row, col);
                if(gradAngle.x != 0 && gradAngle.y != 0)
                    grad_th = atan2(gradAngle.y, gradAngle.x) * 180 / PI;
               
                if(toCenter_th < 0)
                    toCenter_th = 360 + toCenter_th;

                if(abs(grad_th-toCenter_th) < 20)
                    angleOk.at<uchar>(row - center.x + maxR, col - center.y + maxR) = 255;
            }
        }

        imshow("org", src_gray);
        imshow("ANGLE OKKKKs", angleOk);
        waitKey(0); 
        */
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
