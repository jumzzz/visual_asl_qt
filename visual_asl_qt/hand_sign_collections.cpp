#include "hand_sign_collections.h"

// this is for circle

Circle::Circle()
{

}

Circle::Circle(const cv::Point &centroid, const float &radius)
{
    this->centroid = centroid;
    this->radius = radius;
}


cv::Point Circle::getCentroid()
{
    return centroid;
}

float Circle::getRadius()
{
    return radius;
}
