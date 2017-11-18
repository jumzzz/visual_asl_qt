#ifndef HAND_SIGN_COLLECTIONS_H
#define HAND_SIGN_COLLECTIONS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"

#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>

class Circle
{

public:

    explicit Circle(const cv::Point &centroid, const float &radius);
    cv::Point getCentroid();
    float getRadius();

    cv::Point centroid;
    float radius;

private:
    explicit Circle();


};

#endif // HAND_SIGN_COLLECTIONS_H
