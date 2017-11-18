#ifndef HANDLOC_H
#define HANDLOC_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"

#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>

#include "compute.h"



class HandLoc
{
public:
//    HandLoc();
    HandLoc();
    std::vector<double> features(std::vector<cv::Point> contour, cv::RotatedRect rect, int n);

private:



};

#endif // HANDLOC_H
