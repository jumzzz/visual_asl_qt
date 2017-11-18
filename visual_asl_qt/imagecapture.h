#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QDialog>

#include "core_header.h"
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class ImageCapture;
}



class ImageCapture : public QDialog
{
    Q_OBJECT

public:
    explicit ImageCapture(QWidget *parent = 0);
    void showCapture(const cv::Mat &img);
    void setFileManager(FileManager *fileManager);
    void setSkinDetector(SkinDetect *skinDetect);


    ~ImageCapture();

private slots:
    void on_label_outImg_mousePressed(const QPoint &loc);
    void on_label_outImg_mouseReleased(const QPoint &loc);
    void on_btn_addSamples_clicked();
    void on_btn_undoSamples_clicked();
    void on_btn_save_clicked();
    void on_btn_cancel_clicked();
    void on_cb_colorSpace_currentIndexChanged(const QString &arg1);
    void on_cb_trainMode_currentIndexChanged(const QString &arg1);
    void on_cb_colorSpace_activated(const QString &arg1);
    void on_btn_saveHandSeg_clicked();
    void on_btn_Train_clicked();
    void on_btn_deleteAll_clicked();

private:
    Ui::ImageCapture *ui;
    ClickLabel *label_outImg;
    DataManager *m_data_man;
    FileManager *m_fileManager;
    SkinDetect *m_skinDetect;

    bool checkROIValidity(const cv::Point &p0, const cv::Point &p1);
    cv::Rect generateROI(const cv::Point &p0, const cv::Point &p1);

    void skin_mousePressed_Action(const QPoint &loc);
    void skin_mouseReleased_Action(const QPoint &loc);

    void handSeg_mousePressed_Action(const QPoint &loc);
    void handSeg_mouseReleased_Action(const QPoint &loc);

    void m_setImgMatrix(const cv::Mat &img);
    void m_clearData();
    cv::Mat m_currMat;

    QPoint m_qPt0;
    QPoint m_qPt1;

    cv::Point m_cvPt0;
    cv::Point m_cvPt1;

    cv::Scalar m_handROIColor;
    cv::Scalar m_bgROIColor;

    std::map<QString,std::vector<cv::Mat>> m_skin_map;
    std::map<QString,std::vector<cv::Mat>> m_nskin_map;

    std::map<QString, int> m_swIndex_map;

    std::stack<cv::Mat> m_skinCrop;
    std::stack<cv::Mat> m_nSkinCrop;


    const QString KEY_BGR = "BGR";
    const QString KEY_RGB = "RGB";
    const QString KEY_HSV = "HSV";
    const QString KEY_HSV_FULL = "HSV FULL";
    const QString KEY_LUV = "LUV";
    const QString KEY_LAB = "LAB";

    const QString TRAIN_SKIN = "Skin";
    const QString TRAIN_HANDSEG = "Hand Segmentation";

    const QString FILE_WBDETECT = "wb_detector.xml";

    std::vector<cv::Rect> handROI;
    std::vector<cv::Rect> bgROI;


    void m_setCurrMat(const cv::Mat &img);

    // TEST METHOD

    cv::Vec3b m_averageIntensity(const cv::Mat &img);


};

#endif // IMAGECAPTURE_H
