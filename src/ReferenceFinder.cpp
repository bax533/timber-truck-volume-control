#include "ReferenceFinder.hpp"

namespace TimberControl
{
//FIRST TESTED TM_CC0EFF_N0RMED
Area ReferenceFinder::FindTargets()
{


    std::vector<Point> result;
    
    resize(reference, reference, Size(250, 250));
    //imshow("src", source);
    //waitKey(0);

    Canny(source, source, 50, 200, 3, false);

    // reference is scaled 1 pixel down at each iteration to find best possible match
    while(reference.cols > 80)
    {
        Mat ref_canny; 
        Canny(reference, ref_canny, 50, 200, 3, false);
        
        int templates_found = 0;
        std::vector<Point> curRes;

        Mat res_32f(source.rows - ref_canny.rows + 1, source.cols - ref_canny.cols + 1, CV_32FC1);
        matchTemplate(source, ref_canny, res_32f, TM_CCOEFF_NORMED);

        Mat res;
        res_32f.convertTo(res, CV_8U, 255.0);
        
        int found_here_debug = 0;

        while (true) 
        {
            double minval, maxval, threshold = 30;
            Point minloc, maxloc;
            minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
            if (maxval >= threshold)
            {
                found_here_debug += 1;
                //circle(res, maxloc, reference.cols/2, Scalar(0), FILLED); //mark drawn blob
                
                rectangle(source, Point(
                                std::max(maxloc.x - reference.cols/2, 0),
                                std::max(maxloc.y - reference.rows/2, 0)),
                            Point(
                                std::min(maxloc.x + reference.cols, source.cols),
                                std::min(maxloc.y + reference.rows, source.rows)),
                         Scalar(0),
                         FILLED);

                rectangle(res, Point(
                                std::max(maxloc.x - reference.cols/2, 0),
                                std::max(maxloc.y - reference.rows/2, 0)),
                            Point(
                                std::min(maxloc.x + reference.cols, res.cols),
                                std::min(maxloc.y + reference.rows, res.rows)),
                         Scalar(0),
                         FILLED); 
                
                curRes.push_back(Point(maxloc.x + reference.cols/2, maxloc.y + reference.rows/2));
                result.push_back(Point(maxloc.x + reference.cols/2, maxloc.y + reference.rows/2));
                templates_found++;
            }
            else
                break;
        }

        if(found_here_debug >= 1)
        {
            std::cout<<"currently found positions:\n";
            for(const auto& p : result)
            {
                std::cout<<p.x<<"  <-x  y->  "<<p.y<<"\n";
            }
            std::cout<<"---\n";
            /*
            imshow("currentSource", source);
            waitKey(0);
            destroyWindow("currentSource");
            */
            //DEBUG
        }

        if(result.size() == 4)
            break;

        std::cout<<"number of found: "<<result.size()<<"\n";
        printf("%d cur size\n", reference.cols);
        resize(reference, reference, Size(reference.cols - 1, reference.rows - 1));
    }

    if(result.size() == 4)
    {
        std::cout<<"CORRECTLY FOUND 4 TARGETS\n";
        Area retArea;
        const cv::Point origin(
                    (result[0].x + result[1].x + result[2].x + result[3].x)/4,
                    (result[0].y + result[1].y + result[2].y + result[3].y)/4
                );
        std::sort(result.begin(), result.end(), [&origin](const cv::Point &a, const cv::Point &b) {
                    return -atan2(a.x-origin.x,-(a.y-origin.y)) < -atan2(b.x-origin.x, -(b.y-origin.y));
            }
        );

        std::cout<<"AREA DEBUG:\n";
        for(const auto& p : result)
            std::cout<<p.x<<" "<<p.y<<"\n";

        return {
            result[2],
            result[1],
            result[3],
            result[0]};    
    }
    else
    {
        std::cout<<"FOUND "<<result.size()<<" TARGETS, SEARCHING FOR CIRCLES IN WHOLE PHOTO\n";
        return {{0,0},
                {source.cols, 0},
                {0, source.rows},
                {source.cols, source.rows}};
    }
}

}
