#ifndef MAT_CONVERTER_H
#define MAT_CONVERTER_H


// Be careful with this
// Don't use any Qt data structs here

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <vector>
#include <array>
#include <string>
#include <map>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>

class MatConverter
{
public:

    explicit MatConverter();
    explicit MatConverter(const cv::Mat &img);
//    ~MatConverter();

    cv::UMat getBGR();
    cv::UMat getRGB();
    cv::UMat getHSV();
    cv::UMat getHSVFull();
    cv::UMat getLUV();
    cv::UMat getLab();
    cv::UMat getLuvHSV();
    cv::UMat getLabHSV();


private:

    cv::UMat m_bgr;
    cv::UMat m_rgb;
    cv::UMat m_hsv;
    cv::UMat m_hsv_full;
    cv::UMat m_cie_luv;
    cv::UMat m_cie_lab;

    cv::UMat m_luv_hsv;
    cv::UMat m_lab_hsv;

    void m_cvtColor(const cv::Mat &img);


};

#endif // MAT_CONVERTER_H
