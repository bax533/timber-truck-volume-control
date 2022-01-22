#include  "CameraHandler.hpp"
#include <cassert>

CameraHandler::CameraHandler()
{
    initParams_.depth_mode = sl::DEPTH_MODE::ULTRA;
    initParams_.coordinate_units = sl::UNIT::CENTIMETER;
}

sl::Mat CameraHandler::GetImage()
{
    auto returned_state = zed_.open(initParams_);
    assert(returned_state == sl::ERROR_CODE::SUCCESS);
    
    zed_.setCameraSettings(sl::VIDEO_SETTINGS::BRIGHTNESS, 80);
    zed_.setCameraSettings(sl::VIDEO_SETTINGS::SATURATION, 100);

    sl::RuntimeParameters runtimeParams;
    runtimeParams.sensing_mode = sl::SENSING_MODE::STANDARD;

    if(zed_.grab(runtimeParams) == sl::ERROR_CODE::SUCCESS)
    {
        zed_.retrieveImage(image_, sl::VIEW::LEFT);
        zed_.retrieveMeasure(depth_, sl::MEASURE::DEPTH);

        zed_.close();
        return image_;
    }
    else
    {
        zed_.close();
        exit(0);
    }
}

double CameraHandler::GetDistance(int x, int y)
{
    float distance;
    depth_.getValue(x, y, &distance);
    return distance;
}
