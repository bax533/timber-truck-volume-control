#include "ImageHandler.hpp"


namespace TimberControl
{
    class VolumeCalculator
    {
        const char *imgPath;
        ImageHandler imageHandler;

    public:
        VolumeCalculator(const char* imgPath);
      
        double Calculate();
  };
}
