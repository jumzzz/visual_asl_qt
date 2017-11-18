#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QtSql>
#include <QDir>
#include <vector>
#include <map>
#include <QString>
#include <QStringList>
#include <QVariantList>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"

class DataManager
{
public:
    explicit DataManager();

    const QString TABLE_HSV_FULL = "HSV_FULL_SKIN_8U";
    const QString H_HUE = "hue";
    const QString H_SAT = "sat";
    const QString H_VAL = "val";
    const QString H_TYPE = "type";

    void updateHSVValues(const cv::Mat &img, const bool &skin = true);
    void updateHSVValues(const std::vector<cv::Vec3b> &values, const bool &skin = true);
    void updateHSVRow(const cv::Vec3b &values, const bool &skin = true);

private:
    void m_initDB();
    void m_createColorTable();
    bool m_checkColorTable();

    QSqlDatabase m_mainDB;

};

#endif // DATAMANAGER_H
