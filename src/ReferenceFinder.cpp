#include "ReferenceFinder.hpp"

namespace TimberControl
{

std::vector<Point> ReferenceFinder::FindTargets()
{
    std::vector<Point> result;
    
    //imshow("src", source);
    //waitKey(0);

    Canny(source, source, 50, 200, 3, false);

    // reference is scaled 1 pixel down at each iteration to find best possible match
    while(reference.cols > 20)
    {
        Mat ref_canny; 
        Canny(reference, ref_canny, 50, 200, 3, false);
        
        int templates_found = 0;
        std::vector<Point> curRes;

        Mat res_32f(source.rows - ref_canny.rows + 1, source.cols - ref_canny.cols + 1, CV_32FC1);
        matchTemplate(source, ref_canny, res_32f, TM_CCOEFF_NORMED);

        Mat res;
        res_32f.convertTo(res, CV_8U, 255.0);
        
        while (true) 
        {
            double minval, maxval, threshold = 100;
            Point minloc, maxloc;
            minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);
            printf("%lf maxVal\n", maxval);
            if (maxval >= threshold)
            {
                circle(res, maxloc, 32, Scalar(0), FILLED); //mark drawn blob
                curRes.push_back(maxloc);
                templates_found++;
            }
            else
                break;
        }

        if(templates_found == 4)
        {
            result = curRes;
            
            //DEBUG
            {
                Mat ref_canny; 
                Canny(reference, ref_canny, 50, 200, 3, false);
                
                int templates_found = 0;
                std::vector<Point> curRes;

                Mat res_32f(source.rows - ref_canny.rows + 1, source.cols - ref_canny.cols + 1, CV_32FC1);
                matchTemplate(source, ref_canny, res_32f, TM_CCOEFF_NORMED);

                Mat res;
                res_32f.convertTo(res, CV_8U, 255.0);
               
                imshow("found", res);
                waitKey(0); 
            }
            break;
        }
        printf("%d cur size\n", reference.cols);
        resize(reference, reference, Size(reference.cols - 1, reference.rows - 1));
    }

    //assert(result.size() == 4);
    //imshow("final", source);
    //waitKey(0);

    return result;
}

}
