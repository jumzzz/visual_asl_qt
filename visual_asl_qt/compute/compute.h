#ifndef COMPUTE_H
#define COMPUTE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <stack>
#include <iterator>
#include <numeric>
#include <math.h>
#include <set>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/line_descriptor.hpp"



#include "hand_sign_collections.h"

using cv::line_descriptor::KeyLine;



struct KPHist
{
    std::vector<float> magles;
    std::vector<float> angles;
};

///
/// \brief The CVMisc class
/// C++ class contains miscelleneous functions that are necessary for performing
/// computations and other miscelleneous functions

class CVMisc
{
public:

    // Extra statistical functions
    static double dist(cv::Point pt0, cv::Point pt1);
    static double sum(std::vector<double> nums);
    static double mean(std::vector<double> nums);
    static double std_dev(std::vector<double> nums);

    // Extra geometrical functions
    static std::vector<cv::Point> pointGen(const cv::Point &pt0, const cv::Point &pt1, const int &N);

    static bool pointInRect(const cv::RotatedRect &rect, const cv::Point &pt);
    static bool pointInRect(const cv::Rect &rect, const cv::Point &pt);
    static bool pointInRect(const std::vector<cv::Point> &rectPts, const cv::Point &pt);
    static bool pointInRect(const cv::Rect2d &rect, const cv::KeyPoint &kp);

    static bool allKPsInRect(const cv::Rect2d &rect, const std::vector<cv::KeyPoint> &kps);


    static bool keypointInRect(const cv::Rect &rect, const cv::KeyPoint &kp);

    static bool pointInCircle(const float &circleRadius, const cv::Point &circleCentroid, const cv::Point &testPoint);
    static bool pointInCircle(Circle &circle, const cv::Point &testpoint);
    static void clusterPairOfPoints(Circle &c0, Circle &c1, std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1);
    static void clusterPairOfPoints(const cv::Rect &r0, const cv::Rect &r1, const std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1);
    static void clusterPairOfPoints(const std::vector<cv::Point> &r0, const std::vector<cv::Point> &r1, const std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1);

    static int dotProduct(const cv::Point &pt0, const cv::Point &pt1);
    static float dotProduct(const cv::KeyPoint &pt0, const cv::KeyPoint &pt1);

    static int perpDotProduct(const cv::Point &pt0, const cv::Point &pt1);

    static float maxDist(std::vector<cv::Point> vertices);
    static std::pair<std::vector<cv::Point>, std::vector<cv::Point>> extractSubRectPairs(const cv::RotatedRect &rect);

    static std::vector<cv::Rect> generateSubRects(const cv::Rect &rect, int rows, int cols);
    static std::vector<std::vector<cv::Point>> generateSubRects(const std::vector<cv::Point> &rect, int numSides);



    static bool intersection(const cv::Point2f &A1, const cv::Point2f &A2, const cv::Point2f &B1, const cv::Point2f &B2, cv::Point2f &result);

    static std::vector<float> sideMagnitude(const cv::Rect &rect, const std::vector<KeyLine> &lines, float minThresh);
    static std::vector<float> sideMagnitude(const cv::Rect &rect, const std::vector<std::pair<cv::Point, cv::Point>> &lines, float minThresh);

    static float orbDensity(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kp);

    static float aveKeyPointAngle(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kp);
    static float aveKeyPointResp(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kp);
    static float aveKeyPointSize(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kp);

    static std::vector<float> elipticalFeature(const cv::Rect &rect, const std::vector<std::pair<cv::Point, cv::Point>> &lines, cv::RotatedRect &ellipse);

    static cv::Point pointInBet(const cv::Point &p0, const cv::Point &p1, const float &ratio);
    static cv::Point centroid(const std::vector<cv::Point> &points);
    static bool roiValid(const cv::Mat &toBeCropped, const cv::Rect &croppingROI);
    static void correctRect(const cv::Mat &toBeCropped, const cv::Rect &inputROI, cv::Rect &correctedROI);

    static float rectIntersectArea(const cv::Rect2d &rect0, const cv::Rect2d &rect1);




    // Extra Drawing functions
    static void drawKeyLines(cv::Mat &outImg, const std::vector<KeyLine> &keyLines, const cv::Scalar &color, const int &thickness);
    static void drawLines(cv::Mat &outImg, const std::vector<std::pair<cv::Point, cv::Point>> &lines, const cv::Scalar &color, const int &thickness);

    static void drawRotRect(cv::Mat &outImg, const cv::RotatedRect &rect, cv::Scalar color, int thickness);
    static void drawConvex(cv::Mat &outImg, std::vector<cv::Point> &points, cv::Scalar color, int thickness);

    // Filter functions
    static void filterKeyLines(const cv::RotatedRect &rect, const std::vector<KeyLine> &keyLines, std::vector<KeyLine> &outKeyLines);
    static void filterKeyPoints(const cv::RotatedRect &rect, const std::vector<cv::KeyPoint> &kps, std::vector<cv::KeyPoint> &out_kps);

    static cv::Mat cropRotRect(const cv::Mat &src, const cv::RotatedRect &rect);


private:
    CVMisc();
};

#endif // COMPUTE_H
