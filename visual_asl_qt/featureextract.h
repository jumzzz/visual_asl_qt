#ifndef FEATUREEXTRACT_H
#define FEATUREEXTRACT_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/objdetect.hpp"

#include "opencv2/line_descriptor.hpp"

#include "compute/compute.h"

#include <QDebug>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::line_descriptor;
using namespace std;


class Features
{
public:
   static void handToArmSegFeatures(const Mat &inputImg, const Rect &roi, Mat &descriptor);

   static void lineDescriptorFeatures(const Mat &inputImg, const Rect &rect, vector<pair<Point,Point>> &keyLines);
   static void lineDescriptorFeatures(const Mat &inputImg, vector<KeyLine> &keyLines);

   static void ORBDescriptor(const Mat &inputImg, vector<KeyPoint> &keyPoints);

   static vector<float> angularProfileRotRect(const RotatedRect &rect, const vector<KeyLine> &keyLines, int rows, int cols);
   static vector<float> sideMagnitudeFeatures(const Rect &rect, const vector<KeyLine> &keyLines, int rows, int cols);
   static vector<float> sideMagnitudeFeatures(const Rect &rect, const vector<pair<Point,Point>> &lines, int rows, int cols);

   static vector<float> keypointDensities(const Rect &rect, const vector<KeyPoint> &kps, int rows, int cols);
   static vector<float> keypointDensities(const RotatedRect &rect, const vector<KeyPoint> &kps);


//   static vector<float> keypointDensities(const Rect &rect, const vector<KeyPoint> &kps, int rows, int cols);
   static vector<float> keypointDensities(const RotatedRect &rect, const vector<KeyPoint> &kps, vector<KeyPoint> &outKPs);


   static vector<float> ellipticFeaturesTotal(const Rect &rect, const vector<pair<Point, Point>> &lines, int rows, int cols, vector<RotatedRect> &ellipses);
//   static std::vector<float> kpHist(const vector<KeyPoint> &kps, int numOfBins);
   static KPHist kpHist(const vector<KeyPoint> &kps, int numOfBins);

   static vector<float> hogDescriptor(const cv::Mat &input);







};

#endif // FEATUREEXTRACT_H
