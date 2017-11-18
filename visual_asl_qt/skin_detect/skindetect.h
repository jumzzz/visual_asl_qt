#ifndef SKINDETECT_H
#define SKINDETECT_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

#include <QString>



#include "../data_manager/filemanager.h"
#include "../mat_cvt/mat_converter.h"

using std::vector;

// Avoid using Qt Libs here
// Use only Native Libs and OpenCV 3.1+




enum SkinSpace
{
    BGR,RGB,HSV,HSV_FULL,LAB,LUV
};


class SkinDetect
{
public:
    SkinDetect();
//    SkinDetect(FileManager *fm, int nbins);
    void activate(FileManager *fm, int nbins);

    void filterSkin(MatConverter &input, cv::Mat &out, int thresh, SkinSpace ss);
    void filterSkin(MatConverter &input, cv::Mat &out, SkinSpace ss);
    void filterSkin(cv::Mat &input, cv::Mat &out, int thresh);


private:

    const ushort MAX_SKIN_INDEX = 256;
    const ushort MAX_BRINGDOWN_INDEX = 500;

    int m_nbins = 8;

    void filterFromLUT(cv::Mat &input, cv::Mat &output, vector<vector<vector<ulong>>> &table, int thresh);
    void filterFromLUT(cv::Mat &input, cv::Mat &output, vector<vector<vector<ulong> > > &stable, vector<vector<vector<ulong> > > &ntable, float ts, float tn,float pskin, float pnskin);

    void scaleToHist(vector<cv::Vec3i> &input, vector<cv::Vec3i> &output);
    void updateLUT(const vector<cv::Vec3i> &input, vector<vector<vector<ulong>>> &table);


    /// THESE ARE LOOK-UP TABLES

    vector<vector<vector<ulong>>> skin_bgr;
    vector<vector<vector<ulong>>> skin_rgb;
    vector<vector<vector<ulong>>> skin_hsv;
    vector<vector<vector<ulong>>> skin_hsvFull;
    vector<vector<vector<ulong>>> skin_lab;
    vector<vector<vector<ulong>>> skin_luv;

    vector<vector<vector<ulong>>> nskin_bgr;
    vector<vector<vector<ulong>>> nskin_rgb;
    vector<vector<vector<ulong>>> nskin_hsv;
    vector<vector<vector<ulong>>> nskin_hsvFull;
    vector<vector<vector<ulong>>> nskin_lab;
    vector<vector<vector<ulong>>> nskin_luv;
    vector<ushort> bringdown_table;


    void m_init3DTable(vector<vector<vector<ulong>>> &table);
    void m_init3DLUT();
    int m_bringdown(ushort val);
    void m_initBringdownTable();
    int getBringDown(ushort val);


    const QString KEY_BGR = "BGR";
    const QString KEY_RGB = "RGB";
    const QString KEY_HSV = "HSV";
    const QString KEY_HSV_FULL = "HSV FULL";
    const QString KEY_LAB = "LAB";
    const QString KEY_LUV = "LUV";

    float ts_bgr = 0.0f;
    float ts_rgb = 0.0f;
    float ts_hsv = 0.0f;
    float ts_hsvFull = 0.0f;
    float ts_lab = 0.0f;
    float ts_luv = 0.0f;

    float tn_bgr = 0.0f;
    float tn_rgb = 0.0f;
    float tn_hsv = 0.0f;
    float tn_hsvFull = 0.0f;
    float tn_lab = 0.0f;
    float tn_luv = 0.0f;


    float ps_bgr = 0.0f;
    float ps_rgb = 0.0f;
    float ps_hsv = 0.0f;
    float ps_hsvFull = 0.0f;
    float ps_lab = 0.0f;
    float ps_luv = 0.0f;


    float pn_bgr = 0.0f;
    float pn_rgb = 0.0f;
    float pn_hsv = 0.0f;
    float pn_hsvFull = 0.0f;
    float pn_lab = 0.0f;
    float pn_luv = 0.0f;


};

#endif // SKINDETECT_H
