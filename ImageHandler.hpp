#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

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
        n_grad_xy_thin
    };

    class ImageHandler
    {
        Mat src, src_gray, grad_x, grad_y, grad_xy, grad_xy_thin;
        int ksize, scale, delta;
    public:
        ImageHandler() = default;
        ImageHandler(const char* imgPath);
        ImageHandler(const char* imgPath, int ksize, int scale, int delta);
   

        void Prepare(); 
        void showImage(imgNum img); 
    };
}
