#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"


class FileManager
{ 
public:
    explicit FileManager();
    void saveSkinRawData(const std::map<QString, std::vector<cv::Mat>> &skinData, bool skin);
    void deleteAllSkinFiles();

    std::map<QString, std::vector<cv::Vec3i>> extractRawSkinData();
    std::map<QString, std::vector<cv::Vec3i>> extractRawNSkinData();

    QStringList extractRawStrList(const QString &path);
    std::vector<cv::Vec3i> extractDataFromList(QStringList &rawList, bool isSkin);

    QString get_dirPathSegmentHand();
    QString get_dirPathSegmentHandGS();
    QString get_dirPathSegmentHandLaplacian();
    QString get_dirPathSegmentHandSobel();
    QString get_dirPathSegmentHandCanny();

    QString get_dirPathSegmentBG();
    QString get_dirPathSegmentBGGS();
    QString get_dirPathSegmentBGLaplacian();
    QString get_dirPathSegmentBGSobel();
    QString get_dirPathSegmentBGCanny();





    ~FileManager();

private:

    QStringList matToStringList(const std::vector<cv::Mat> &matList, bool skin);
    void appendListToFile(const QStringList &list, QFile &file);

    QString dirPath_app;
    QString dirPath_skin_bgr;
    QString dirPath_skin_rgb;
    QString dirPath_skin_hsv;
    QString dirPath_skin_hsvFull;
    QString dirPath_skin_lab;
    QString dirPath_skin_luv;

    QString filePath_skin_bgr;
    QString filePath_skin_rgb;
    QString filePath_skin_hsv;
    QString filePath_skin_hsvFull;
    QString filePath_skin_lab;
    QString filePath_skin_luv;


    QString dirPathSegmentHand;
    QString dirPathSegmentHandGS;
    QString dirPathSegmentHandLaplacian;
    QString dirPathSegmentHandSobel;
    QString dirPathSegmentHandCanny;

    QString dirPathSegmentBG;
    QString dirPathSegmentBGGS;
    QString dirPathSegmentBGLaplacian;
    QString dirPathSegmentBGSobel;
    QString dirPathSegmentBGCanny;

    QDir dirSegmentHand;
    QDir dirSegmentHandGS;
    QDir dirSegmentHandLaplacian;
    QDir dirSegmentHandSobel;
    QDir dirSegmentHandCanny;

    QDir dirSegmentBG;
    QDir dirSegmentBGGS;
    QDir dirSegmentBGLaplacian;
    QDir dirSegmentBGSobel;
    QDir dirSegmentBGCanny;

//    QString dirPathSegmentBG;
//    QString dirPathSegmentBGGS;


    const QString SEGMENT= "Segment";
    const QString SEGMENT_HAND = "Hand";
    const QString SEGMENT_BG = "BG";

    const QString SEGMENT_GS = "/GrayScale/";
    const QString SEGMENT_LAPLACIAN = "/Laplacian/";
    const QString SEGMENT_SOBEL = "/Sobel/";
    const QString SEGMENT_CANNY = "/Canny/";


    QDir dirBGR;
    QDir dirRGB;
    QDir dirHSV;
    QDir dirHSV_FULL;
    QDir dirLAB;
    QDir dirLUV;


//  This private fields are too dangerous, potential dangling pointer
//    QFile *fileBGR;
//    QFile *fileRGB;
//    QFile *fileHSV;
//    QFile *fileHSV_FULL;
//    QFile *fileLAB;
//    QFile *fileLUV;

    void m_mkSkinDir();
    void m_mkSkinFiles();

    void m_mkHandDetectDir();
    void m_mkHandDetectFiles();

    const QString KEY_BGR = "BGR";
    const QString KEY_RGB = "RGB";
    const QString KEY_HSV = "HSV";
    const QString KEY_HSV_FULL = "HSV FULL";
    const QString KEY_LAB = "LAB";
    const QString KEY_LUV = "LUV";



};

#endif // FILEMANAGER_H
