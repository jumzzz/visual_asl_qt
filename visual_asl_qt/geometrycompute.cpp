#include "geometrycompute.h"

GeometryCompute::GeometryCompute()
{

}

cv::Rect GeometryCompute::genCropRect(const cv::Point &pt0, const cv::Point &pt1)
{
     int x0 = pt0.x;
     int y0 = pt0.y;
     int x1 = pt1.x;
     int y1 = pt1.y;

     int w = abs(x1 - x0);
     int h = abs(y1 - y0);

     if (x0 < x1 && y0 < y1)
        return cv::Rect(x0, y0, w, h);
     else if (x0 > x1 && y0 < y1)
        return cv::Rect(x1,y0,w,h);
     else if (x0 < x1 && y0 > y1)
        return cv::Rect(x0,y1,w,h);
     else if (x0 > x1 && y0 > y1)
        return cv::Rect(x1,y1,w,h);
     else
        return cv::Rect();

}
