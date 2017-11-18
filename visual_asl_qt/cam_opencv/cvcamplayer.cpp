#include "cvcamplayer.h"


CVCamPlayer::CVCamPlayer(QObject *parent)
    :QThread(parent)
{
    m_stop = true;
}


bool CVCamPlayer::initCam()
{
    m_cv_capture.open(1);

    if (m_cv_capture.isOpened())
    {
        m_frameRate = (int)m_cv_capture.get(CV_CAP_PROP_FPS);

        m_cv_capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
        m_cv_capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);

        m_cv_capture.set(CV_CAP_PROP_FPS, 60);
//        m_cv_capture.set(CV_CAP_PROP_EXPOSURE, 0);


        return true;
    }
    else
        return false;

}

void CVCamPlayer::play()
{
    if(!isRunning())
    {
        if(isStopped())
            m_stop = false;

//        start(LowPriority);
        start(HighPriority);
    }
}

void CVCamPlayer::run()
{
    int delay = (1000/m_frameRate);

    while(!m_stop)
    {
        if(!m_cv_capture.read(m_cv_frame))
        {
            m_stop = true;
        }

//        int exposure = (int)m_cv_capture.get(CV_CAP_PROP_EXPOSURE);
//        qDebug() << "Exposure Constant: " << exposure;
        emit processCVMat(m_cv_frame.clone());
        this->msleep(delay);
    }

}

CVCamPlayer::~CVCamPlayer()
{
    m_qt_mutex.lock();
    m_stop = true;
    m_cv_capture.release();
    m_qt_condition.wakeOne();
    m_qt_mutex.unlock();
    wait();
}

void CVCamPlayer::stop()
{
    m_stop = true;
//    m_cv_capture.release();

}

void CVCamPlayer::msleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep((uint)ms);
#endif

#ifdef Q_OS_LINUX
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

bool CVCamPlayer::isStopped() const
{
    return this->m_stop;
}
