#include "mat_converter.h"

MatConverter::MatConverter()
{

}

MatConverter::MatConverter(const cv::Mat &img)
{
    m_cvtColor(img);
//    m_cvtMatQImage();
}

void MatConverter::m_cvtColor(const cv::Mat &img)
{
//    cv::Mat imgFloat(img.rows,img.cols, CV_8U);
//    img.convertTo(imgFloat, CV_8U);
    m_bgr = img.getUMat(cv::ACCESS_FAST).clone();

    cv::cvtColor(img, m_rgb, CV_BGR2RGB);
    cv::cvtColor(img, m_hsv_full, CV_BGR2HSV_FULL);
    cv::cvtColor(img, m_hsv, CV_BGR2HSV);
    cv::cvtColor(img, m_cie_luv, CV_BGR2Luv);
    cv::cvtColor(img, m_cie_lab, CV_BGR2Lab);

    cv::cvtColor(m_cie_luv, m_luv_hsv, CV_BGR2HSV_FULL);
    cv::cvtColor(m_cie_lab, m_lab_hsv, CV_BGR2HSV_FULL);
}

cv::UMat MatConverter::getLabHSV()
{
    return m_lab_hsv;
}

cv::UMat MatConverter::getLuvHSV()
{
    return m_luv_hsv;
}

cv::UMat MatConverter::getBGR()
{
    return m_bgr;
}

cv::UMat MatConverter::getRGB()
{
    return m_rgb;
}

cv::UMat MatConverter::getHSVFull()
{
    return m_hsv_full;
}

cv::UMat MatConverter::getHSV()
{
    return m_hsv;
}

cv::UMat MatConverter::getLUV()
{
    return m_cie_luv;
}

cv::UMat MatConverter::getLab()
{
    return m_cie_lab;
}





