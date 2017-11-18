#ifndef GEOMETRYCOMPUTE_H
#define GEOMETRYCOMPUTE_H

//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "core_header.h"

class GeometryCompute
{
public:
    GeometryCompute();
    cv::Rect genCropRect(const cv::Point &pt0, const cv::Point &pt1);
};

#endif // GEOMETRYCOMPUTE_H
