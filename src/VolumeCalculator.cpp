#include "VolumeCalculator.hpp"
#include <fstream>
#include <cstdio>

namespace TimberControl
{

    VolumeCalculator::VolumeCalculator(cv::Mat image)
    {
        imageHandler = ImageHandler(image);
        imageHandler.Prepare();
    }

    VolumeCalculator::VolumeCalculator(const char* imgPath)
        : imgPath(imgPath)
    {
        imageHandler = ImageHandler(imgPath);
        imageHandler.Prepare();
    }
   
    //distance in [m] 
    void VolumeCalculator::Start(std::string referencePath, double camToTrailerBegin_dist, double distBetweenTargets_vert)
    {
        //Take photo with camera and save it
        CameraHandler camHandler = CameraHandler();

       //READ Z KAMERKI
       /* 
        sl::Mat slImg = camHandler.GetImage();
        cv::Mat cameraImg = TimberControl::slMat2cvMat(slImg);
        
        imshow("camera image", cameraImg);
        waitKey(0);
        */
        //cv::imwrite("saved_with_4reference.png", cameraImg);       
       
        //READ NA SZTYWNO 
        cv::Mat cameraImg = imread("FIRST.png");
        
        imshow("camImage", cameraImg);
        waitKey(0);
        destroyWindow("camImage");
        
                //Try to find reference images in the photo
        TimberControl::ReferenceFinder refFinder(referencePath.c_str(), cameraImg);
        Area searchArea = refFinder.FindTargets();
        
        circle(cameraImg, searchArea.upper_l, 20, Scalar(0,0,255), FILLED);
        circle(cameraImg, searchArea.upper_r, 20, Scalar(0,0,255), FILLED);
        circle(cameraImg, searchArea.lower_l, 20, Scalar(0,0,255), FILLED);
        circle(cameraImg, searchArea.lower_r, 20, Scalar(0,0,255), FILLED);

        imshow("with found", cameraImg);
        //imwrite("found_screen.png", cameraImg);
        std::cout<<"ESC to continue\n";
        while(waitKey(0) != 27)
            ;

        destroyWindow("with found");

        imageHandler = ImageHandler(cameraImg);
        imageHandler.Prepare();

        imshow("GRAY",imageHandler.src_gray);
        waitKey(0);
        destroyWindow("GRAY");

        imshow("CANNY", imageHandler.grad_xy_thin);
        waitKey(0);
        destroyWindow("CANNY");

        imshow("phase", imageHandler.phase_img);
        waitKey(0);
        destroyWindow("phase");



        std::cout<<"RECEIVED AREA:\n";
        std::cout<<"UPPER LEFT: "<<searchArea.upper_l<<"\n";
        std::cout<<"UPPER RIGHT: "<<searchArea.upper_r<<"\n"; 
        std::cout<<"LOWER LEFT: "<<searchArea.lower_l<<"\n";
        std::cout<<"LOWER RIGHT: "<<searchArea.lower_r<<"\n";

        std::vector<Circle> foundCircles = imageHandler.FindCircles(searchArea);
        std::cout<<foundCircles.size()<<" found circles\n";
       

        cv::Mat foundCircles_img(cameraImg.rows, cameraImg.cols, CV_8U, Scalar(0));
        
        //cv::Mat labeledImg = imread("wood_labels.png", cv::IMREAD_GRAYSCALE);            

        for(const Circle& circle : foundCircles)
        {
            cv::circle(foundCircles_img, circle.center, circle.r, Scalar(255), FILLED);
        }
       
        //imshow("source", cameraImg);
    
        imshow("circles", foundCircles_img);
        imshow("source", cameraImg);        
        imwrite("found_iphone.jpg", foundCircles_img); 
        waitKey(0);
        /* Mat AND, counting pixels DEBUG
        
        cv::Mat labeled_and_found(labeledImg.rows, labeledImg.cols, CV_8U);
        bitwise_and(labeledImg, foundCircles_img, labeled_and_found);
        imshow("found && labels", labeled_and_found); 
        std::cout<<cv::countNonZero(labeled_and_found)<<" <- found && labeled  |  labeled -> "<<cv::countNonZero(labeledImg)<<"\n";
        waitKey(0);
        */

        double px_to_metres = distBetweenTargets_vert/(double)(searchArea.lower_l.y - searchArea.upper_l.y);
        
        double volume = 0.0;

        double D = camToTrailerBegin_dist;
        for(const Circle& circle : foundCircles)
        {
            double circleArea = pow((double)circle.r * px_to_metres, 2) * PI;

            double distToAbut = camHandler.GetDistance(circle.center.x, circle.center.y)/100.0;
            std::cout<<"DISTANCE TO LOG END: "<<distToAbut<<"\n";
            double alpha = horizontalAngleForPixel(circle.center.x, cameraImg.cols, cameraImg.rows);
            double l = D - distToAbut * std::sin(1.5708 - alpha); // 90 deg = 1.5708 rad

            volume += circleArea * l;
        }
        
        std::cout<<volume<<" m3\n";
        waitKey(0);
    }

    double VolumeCalculator::Calculate(const std::vector<Circle>& circles, double px_to_metres)
    {
        double volume = 0.0;
        for(const Circle& circle : circles)
        {
            double circleArea = circle.r * px_to_metres;
            volume += circleArea; 
        }
        
       return volume;
    }

    
    void VolumeCalculator::FindParameters()
    {
        std::vector<std::string> examplePaths = {
            "wood.png",
            "wood_on_truck_1.jpg",
            "wood_on_truck_2.jpg"
        };

        std::vector<std::string> labeledPaths = {
            "wood_labels.png",
            "wood_on_truck_1_label.jpg",
            "wood_on_truck_2_label.png"
        };

        double bestErrVal = 9999999999999.0;

        int bestAngle = -1;
        double bestCircleRatio = -1.0;


        std::ofstream resFile;
        resFile.open("results.csv");

        resFile<<"angle,circleRatio,MSE\n";

        for(int angleVal = 5; angleVal <= 15; angleVal++)
        {
            for(double circleRatioThreshold = 0.5; circleRatioThreshold < 1.51; circleRatioThreshold += 0.05)
            {
                long errSum = 0;
                for(int it = 0; it<examplePaths.size(); it++)
                {
                    cv::Mat cameraImg = imread(examplePaths[it]);
                    
                    imageHandler = ImageHandler(cameraImg);
                    imageHandler.Prepare();
                    std::vector<Circle> foundCircles = imageHandler.FindCirclesDebug(angleVal, circleRatioThreshold);
                    
                    cv::Mat foundCircles_img(cameraImg.rows, cameraImg.cols, CV_8U, Scalar(0));
                    for(const Circle& circle : foundCircles)
                    {
                        cv::circle(foundCircles_img, circle.center, circle.r, Scalar(255), FILLED);
                    }
                    
                    cv::Mat labeledImg = imread(labeledPaths[it], cv::IMREAD_GRAYSCALE);
                    cv::Mat labeled_and_found(labeledImg.rows, labeledImg.cols, CV_8U);
                    
                    bitwise_and(labeledImg, foundCircles_img, labeled_and_found);
                    //std::cout<<cv::countNonZero(labeled_and_found)<<" <- found && labeled  |  labeled -> "<<cv::countNonZero(labeledImg)<<"\n";
                    long err = std::abs(cv::countNonZero(labeledImg) - cv::countNonZero(labeled_and_found))/100
                        + (cv::countNonZero(foundCircles_img) - cv::countNonZero(labeled_and_found))/100;
                    errSum += err*err;
                }

                double curMSE = (double)errSum/(double)examplePaths.size();

                if(curMSE < bestErrVal)
                {
                    bestErrVal = curMSE; 
                    bestAngle = angleVal;
                    bestCircleRatio = circleRatioThreshold;
                }

                std::cout<<"angleVal "<<angleVal<<"\n";
                std::cout<<"circleRatioThreshold "<<circleRatioThreshold<<"\n";
                std::cout<<"MSE: "<<(double)errSum/(double)examplePaths.size()<<"\n---------\n";
            
                resFile<<angleVal<<","<<circleRatioThreshold<<","<<curMSE<<"\n";
            }
        }

        resFile.close();

        std::cout<<"BEST ANGLE: "<<bestAngle<<"\n"<<"BEST CIRCLE RATIO THRESH: "<<bestCircleRatio<<"\n";
        std::cout<<"BEST MSE: "<< bestErrVal;
    }
}
