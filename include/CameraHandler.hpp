#ifndef CAMERA_HANDLER
#define CAMERA_HANDLER

#include <sl/Camera.hpp>

class CameraHandler
{
public:
    CameraHandler();

    sl::Mat GetImage();
    double GetDistance(int x, int y);
private:

    sl::Camera zed_;
    sl::InitParameters initParams_;

    sl::Mat image_;
    sl::Mat depth_;
};

#endif
