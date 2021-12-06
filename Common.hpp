#ifndef COMMON_H
#define COMMON_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include "math.h"

#define PI 3.1415926535897932384

namespace TimberControl
{
    using namespace cv;

    const int maxR = 44, minR = 8;

    template <typename T>
    struct Vector
    {
        T x, y;
    };

    struct Circle
    {
        std::pair<int, int> center;
        int r;
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

    inline bool InBounds(const std::pair<int, int>& p, const int& r, const int& width, const int& height)
    {
        return p.first - r >= 0 && p.first + r < height &&
            p.second - r >= 0 && p.second + r < width;
    }

    inline bool InBounds(const std::pair<int, int>& p, const int& width, const int& height)
    {
        return p.first >= 0 && p.first < height &&
            p.second >= 0 && p.second < width;
    }

    inline void getMaxAndPos(const Mat& N, const Mat& R, double& maxVal, Point& p)
    {
        double curMax = -1.0;
        for(int row = 0; row < N.rows; row++)
        {
            for(int col = 0; col < N.cols; col++)
            {
                int curN = N.at<int>(row, col);
                int curR = R.at<int>(row, col);

                double curVal = (double)curN/(double)curR;
                if(curVal > curMax)
                {
                    p = Point(row, col);
                    maxVal = curVal;
                    curMax = curVal;
                }
            }
        }
    }
}

#endif
