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
        R = Mat(Size(src.cols, src.rows), CV_32SC1, Scalar(0)); 
        N = Mat(Size(src.cols, src.rows), CV_32SC1, Scalar(0));
        toCenter_mat = Mat(Size(2*maxR, 2*maxR), CV_32F, Scalar(0));

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
        
        for(int row=0; row<2*maxR; row++)
        {
            //float* rPtr = toCenter_mat.ptr<float>(row);
            for(int col = 0; col < 2*maxR; col++)
            {
                float toCenter_th = ApproxAtan2(maxR - row, maxR - col) * toDeg;
                toCenter_mat.at<float>(row, col) = toCenter_th >= 0 ? toCenter_th : 360 + toCenter_th; 
            }
        }

        assert(src.rows == R.rows && src.cols == R.cols && src.rows == grad_x.rows && src.cols == grad_x.cols);
    }

    std::vector<Circle> ImageHandler::FindCircles()
    {
        return FindCircles({
                {0,0},
                {src_gray.cols, 0},
                {0, src_gray.rows},
                {src_gray.cols, src_gray.rows}});
    }

    std::vector<Circle> ImageHandler::FindCircles(Area searchArea)
    {
        int height = grad_x.rows;
        int width = grad_x.cols;

        for(int row = std::min(searchArea.upper_l.y, searchArea.upper_r.y);
                row < std::max(searchArea.lower_l.y, searchArea.lower_r.y);
                row++)
        {   
            for(int col = std::min(searchArea.upper_l.x, searchArea.lower_l.x);
                    col < std::max(searchArea.upper_r.x, searchArea.lower_r.x);
                    col++)
            {
                FindBestCircle(Point(row, col));
            }
            std::cout<<(double)row/(double)height * 100.0<<"%\n";
        }
        
        Mat resultImg = Mat::zeros(src_gray.rows, src_gray.cols, CV_8U);
        double maxVal = -1.0;
        Point maxPoint;
        getMaxAndPos(N, R, maxVal, maxPoint);
        
        std::cout<<"point max: "<<maxPoint.x<<" "<<maxPoint.y<<"\n";
        int curR = (int)R.at<int>(maxPoint.y, maxPoint.x);
        
        std::cout<<maxVal<<"  <- maxVal  cur R -> "<<(int)curR<<"\n"; 

        std::vector<Circle> resultVec;

        while(maxVal > circleRatioThreshold)
        {
            if(curR > maxR)
                continue;
            circle(resultImg, Point(maxPoint.y, maxPoint.x), curR, Scalar(255), FILLED);
            circle(N, Point(maxPoint.y, maxPoint.x), curR, Scalar(0), FILLED);
            
            resultVec.push_back({Point(maxPoint.y, maxPoint.x), curR});
            getMaxAndPos(N, R, maxVal, maxPoint);
            curR = R.at<int>(maxPoint.x, maxPoint.y);

            std::cout<<maxVal<<" <- cur ratio\n";
            std::cout<<curR<<" <-curR  Point-> "<<maxPoint.x<<", "<<maxPoint.y<<"  maxVal:"<<maxVal<<"\n";
        }
        /*
        imshow("kolkas", resultImg);
        imshow("source", src_gray);
        waitKey(0);
        imwrite("kolka.jpg", resultImg);
        imwrite("source.jpg", src_gray);
    
        std::cout<<resultVec.size()<<" found kolkas\n";
        */
        return resultVec; 
    
    }   

    std::vector<Circle> ImageHandler::FindCirclesDebug(int angleThresh, double circleThresh)
    {
        int height = grad_x.rows;
        int width = grad_x.cols;

        for(int row = 0; row < src_gray.rows; row++)
        {   
            for(int col = 0; col < src_gray.cols; col++)
            {
                FindBestCircle(Point(row, col), angleThresh);
            }
        }

        Mat resultImg = Mat::zeros(src_gray.rows, src_gray.cols, CV_8U);
        double maxVal = -1.0;
        Point maxPoint;
        getMaxAndPos(N, R, maxVal, maxPoint);
        
        int curR = (int)R.at<int>(maxPoint.y, maxPoint.x);
        
        std::vector<Circle> resultVec;

        while(maxVal > circleThresh)
        {
            if(curR > maxR)
            {
                circle(N, Point(maxPoint.y, maxPoint.x), 2*minR, Scalar(0), FILLED);
                resultVec.push_back({Point(maxPoint.y, maxPoint.x), curR});
                getMaxAndPos(N, R, maxVal, maxPoint);
                curR = R.at<int>(maxPoint.x, maxPoint.y); 
                continue;
            }    
            circle(resultImg, Point(maxPoint.y, maxPoint.x), curR, Scalar(255), FILLED);
            circle(N, Point(maxPoint.y, maxPoint.x), curR, Scalar(0), FILLED);
            
            resultVec.push_back({Point(maxPoint.y, maxPoint.x), curR});
            getMaxAndPos(N, R, maxVal, maxPoint);
            curR = R.at<int>(maxPoint.x, maxPoint.y);
        }
        return resultVec;  
    }

    void ImageHandler::FindBestCircle(const Point& center_orig, int angleThresh)
    {//TODO adjust finding circle to image edges (currently finding on smaller img)
        int height = grad_x.rows;
        int width = grad_x.cols;

        int rows_from = center_orig.x - maxR < 0 ? 0 : center_orig.x - maxR;
        int rows_to = center_orig.x + maxR >= height ? height : center_orig.x + maxR;
        int cols_from = center_orig.y - maxR < 0 ? 0 : center_orig.y - maxR;
        int cols_to = center_orig.y + maxR >= width ? width : center_orig.y + maxR;

        if(center_orig.x - maxR < 0) return;
        if(center_orig.x + maxR >= height) return;
        if(center_orig.y - maxR < 0) return;
        if(center_orig.y + maxR >= width) return;

        Mat current_phase = phase_img(
                Range(rows_from, rows_to),
                Range(cols_from, cols_to));
        
        Mat current_canny = grad_xy_thin(
                Range(rows_from, rows_to),
                Range(cols_from, cols_to));
        
        Mat result = abs(toCenter_mat - current_phase);
        
        threshold(result, result, angleThresh, 255, 1);
        result.convertTo(result, CV_8U);
        current_canny.convertTo(current_canny, CV_8U);
        
        bitwise_and(result, current_canny, result);
        
        int bestN = -1;
        int bestR = -1;

        for(int r = minR; r <= maxR; r++)
        {
            Mat circleMat = Mat::zeros(2*maxR, 2*maxR, CV_8U);
            circle(circleMat, Point(maxR, maxR), r, Scalar(255));
            bitwise_and(result, circleMat, circleMat);
            int n = countNonZero(circleMat); 
            if(n > bestN)
            {
                bestN = n;
                bestR = r;
            }
        }

        R.at<int>(center_orig.x, center_orig.y) = bestR;
        N.at<int>(center_orig.x, center_orig.y) = bestN; 
        
        assert(center_orig.x >= 0 && center_orig.x < R.rows);
        assert(center_orig.y >= 0 && center_orig.y < N.cols);

        assert(N.rows == R.rows && N.cols == R.cols);

        assert(R.at<int>(center_orig.x, center_orig.y) == bestR);
        assert(N.at<int>(center_orig.x, center_orig.y) == bestN);
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
