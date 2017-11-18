#ifndef CVCAMPLAYER_H
#define CVCAMPLAYER_H

#include "cam_opencv_global.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QVariant>
#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif



class CVCamPlayer:public QThread
{
    Q_OBJECT

signals:
    void processCVMat(const cv::Mat &image);

protected:
    void run();
    void msleep(int ms);

private:
    bool m_stop;
    QMutex m_qt_mutex;
    QWaitCondition m_qt_condition;

    cv::Mat m_cv_frame;
    cv::VideoCapture m_cv_capture;
    cv::Mat m_cv_rgbFrame;

    int m_frameRate;
    QImage m_qt_img;

public:
    CVCamPlayer(QObject *parent = 0);
    ~CVCamPlayer();

    bool initCam();
    void play();
    void stop();

    bool isStopped() const;
};

Q_DECLARE_METATYPE(cv::Mat)


#endif // CVCAMPLAYER_H
