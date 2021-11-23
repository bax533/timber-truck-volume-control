#ifndef COMMON_H
#define COMMON_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "math.h"

#define PI 3.1415926535897932384

namespace TimberControl
{
    using namespace cv;

    template <typename T>
    struct Vector
    {
        T x, y;
    };


    const String windowName = "window";

    template <typename T>
    inline double Dot(const Vector<T>& v,const Vector<T>& g)
    {
        return ((double)v.x * (double)g.x + (double)v.y * (double)g.y);
    }

    template <typename T>
    inline double Mag(const Vector<T>& v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    inline double AngleBetweenVectors(const Vector<int>& v,const Vector<int>& g)
    {
        return acos(Dot<int>(v, g)/Mag<int>(v)*Mag<int>(g));
    }
}

#endif
