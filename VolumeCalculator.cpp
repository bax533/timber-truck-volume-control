#include "VolumeCalculator.hpp"

#include <cstdio>

namespace TimberControl
{
    VolumeCalculator::VolumeCalculator(const char* imgPath)
        : imgPath(imgPath)
    {
        imageHandler = ImageHandler(imgPath);
        imageHandler.Prepare();
    }

    double VolumeCalculator::Calculate()
    {
        printf("%s\n", imgPath);
        
        imageHandler.showImage(n_grad_y);
        return 0.0;
    }
}
