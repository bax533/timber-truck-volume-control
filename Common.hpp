#ifndef COMMON_H
#define COMMON_H

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include "math.h"
#include <cstdint>

#define PI 3.1415926535897932384
#define PI_2 1.57079632679

namespace TimberControl
{
    using namespace cv;

    const int maxR = 46, minR = 8;
    const float toDeg = 180.0/PI;

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

    inline float ApproxAtan2(float y, float x)
    {
        const float n1 = 0.97239411f;
        const float n2 = -0.19194795f;    
        float result = 0.0f;
        if (x != 0.0f)
        {
            const union { float flVal; uint32_t nVal; } tYSign = { y };
            const union { float flVal; uint32_t nVal; } tXSign = { x };
            if (fabsf(x) >= fabsf(y))
            {
                union { float flVal; uint32_t nVal; } tOffset = { PI };
                // Add or subtract PI based on y's sign.
                tOffset.nVal |= tYSign.nVal & 0x80000000u;
                // No offset if x is positive, so multiply by 0 or based on x's sign.
                tOffset.nVal *= tXSign.nVal >> 31;
                result = tOffset.flVal;
                const float z = y / x;
                result += (n1 + n2 * z * z) * z;
            }
            else // Use atan(y/x) = pi/2 - atan(x/y) if |y/x| > 1.
            {
                union { float flVal; uint32_t nVal; } tOffset = { PI_2 };
                // Add or subtract PI/2 based on y's sign.
                tOffset.nVal |= tYSign.nVal & 0x80000000u;            
                result = tOffset.flVal;
                const float z = x / y;
                result -= (n1 + n2 * z * z) * z;            
            }
        }
        else if (y > 0.0f)
        {
            result = PI_2;
        }
        else if (y < 0.0f)
        {
            result = -PI_2;
        }
        return result;
    }
}

#endif
