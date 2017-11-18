#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>

//#include "cam_opencv/cvcamplayer.h"

//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"

#include "core_header.h"

#include "handarmtrainer.h"
#include "opencv2/tracking.hpp"
//#include "opencv2/xobjdetect.hpp"

#include <QList>
#include <QVector>

using cv::xobjdetect::WBDetector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString KEY_BGR = "BGR";
    const QString KEY_RGB = "RGB";
    const QString KEY_HSV = "HSV";
    const QString KEY_HSV_FULL = "HSV FULL";
    const QString KEY_LUV = "LUV";
    const QString KEY_LAB = "LAB";

    const QString FILTER_NORMAL = "Normal";
    const QString FILTER_SKIN = "Skin";

    const QString FILE_WBDETECT = "wb_detector.xml";

private slots:

    void on_receiveCVMat(const cv::Mat &img);
    void on_btn_PlayStop_clicked();

    void on_mouseClick_label_outImg(const QPoint &loc);
    void on_mouseRelease_label_outImg(const QPoint &loc);
    void on_btn_capture_clicked();

//    void on_menuTrain_clicked();
    void on_actionHand_Arm_Detector_toggled(bool toggled);

    void on_m_handArmTrainer_has_closed();
    void on_m_handArmTrainer_has_shown();


private:
    Ui::MainWindow *ui;
    CVCamPlayer *m_camPlayer;
    ClickLabel *m_label_outImg;
    FileManager m_fileManager;
    ImageCapture *m_imageCapture;

    cv::Mat m_currMat;
    SkinDetect m_skinDetect;
    HandSegArmTrainer *m_handArmTrainer;

    // Classifiers
    HandSegArmClassifier m_handSegArmClassifier;

    std::vector<float> m_weights;


    bool m_trackerHasInit = false;

    bool m_WBDetectorFileExist();

    cv::Ptr<cv::BRISK> brisk;
    cv::Ptr<cv::DescriptorMatcher> trackMatcher;


    std::vector<cv::KeyPoint> trackKp0;
    std::vector<cv::KeyPoint> trackKp1;

    cv::Ptr<Tracker> tracker;

    bool m_trackerInit = false;

    cv::Mat trackDesc0;
    cv::Mat trackDesc1;

    cv::Rect2d m_trackRect;



};

#endif // MAINWINDOW_H
