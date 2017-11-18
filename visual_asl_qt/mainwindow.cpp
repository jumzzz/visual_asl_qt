#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_camPlayer = new CVCamPlayer(this);
    //    waldBoost = WBDetector::create();


    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(m_camPlayer, SIGNAL(processCVMat(cv::Mat)), this, SLOT(on_receiveCVMat(cv::Mat)));

    m_label_outImg = new ClickLabel;
    ui->verticalLayout->addWidget(m_label_outImg);
    m_label_outImg->setAlignment(Qt::AlignCenter);
    m_label_outImg->setStyleSheet("Background-color: #000;");

    connect(m_label_outImg, SIGNAL(mousePressed(QPoint)), this, SLOT(on_mouseClick_label_outImg(QPoint)));
    connect(m_label_outImg, SIGNAL(mouseReleased(QPoint)), this, SLOT(on_mouseRelease_label_outImg(QPoint)));

    ui->cb_colorSpace->addItem(KEY_RGB);
    ui->cb_colorSpace->addItem(KEY_BGR);
    ui->cb_colorSpace->addItem(KEY_HSV);
    ui->cb_colorSpace->addItem(KEY_HSV_FULL);
    ui->cb_colorSpace->addItem(KEY_LAB);
    ui->cb_colorSpace->addItem(KEY_LUV);

    ui->cb_filter->addItem(FILTER_NORMAL);
    ui->cb_filter->addItem(FILTER_SKIN);

    m_skinDetect.activate(&m_fileManager, 64);

    m_imageCapture = new ImageCapture(this);
    m_imageCapture->setFileManager(&m_fileManager);
    m_imageCapture->setSkinDetector(&m_skinDetect);

    m_handArmTrainer = new HandSegArmTrainer(this);

    m_handArmTrainer->setSkinDetector(&m_skinDetect);
    m_handArmTrainer->setCamplayer(m_camPlayer);

    connect(m_handArmTrainer, SIGNAL(has_shown()), this, SLOT(on_m_handArmTrainer_has_shown()));
    connect(m_handArmTrainer, SIGNAL(has_closed()), this, SLOT(on_m_handArmTrainer_has_closed()));

    connect(ui->actionHand_Arm_Detector, SIGNAL(triggered(bool)), SLOT(on_actionHand_Arm_Detector_toggled(bool)));


//    m_weights = {0,0,1,0,0,0,1};

    m_handSegArmClassifier.init();

    m_handSegArmClassifier.loadTrainedData();

    m_handArmTrainer->setHandSegArmClassifier(&m_handSegArmClassifier);

    brisk = cv::BRISK::create();

    trackMatcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);

    tracker = cv::Tracker::create("KCF");



}

void MainWindow::on_mouseClick_label_outImg(const QPoint &loc)
{

    if(!m_currMat.empty())
    {

        int w = m_currMat.cols;
        int h = m_currMat.rows;

        QSize matSize(w,h);
        QSize pixSize(m_label_outImg->pixmap()->size());

        qDebug() << "Mat Size: " << matSize;
        qDebug() << "Pix Size: " << pixSize;

    }

}

void MainWindow::on_mouseRelease_label_outImg(const QPoint &loc)
{
    //    qDebug() << "Point Released: " << loc;
}

void MainWindow::on_receiveCVMat(const cv::Mat &img)
{
    if(!img.empty())
    {
        int cols = img.cols;
        int rows = img.rows;

        QImage qtImg;

        m_currMat = img.clone();

        cv::Mat outImg = img.clone();
        cv::Mat hsvImg;
        cv::cvtColor(img.clone(), hsvImg, CV_BGR2HSV_FULL);

        std::vector<cv::Mat> hsvChannels;
        cv::split(hsvImg, hsvChannels);
        cv::Mat valImg = hsvChannels.at(2);
        QString dispMode = ui->cb_filter->currentText();


        cv::Mat skinDetect;
        m_skinDetect.filterSkin(hsvImg,skinDetect,HSV_FULL);
        cv::Mat contourIO = skinDetect.clone();

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(contourIO,contours,CV_RETR_CCOMP, CV_CHAIN_APPROX_TC89_KCOS, cv::Point(2,2));


        std::vector<std::pair<std::vector<cv::Point>, double>> contoursAreas0(contours.size());

        int index = 0;

        for(std::vector<cv::Point> &c : contours)
        {
            double area = cv::contourArea(c);
            contoursAreas0.insert(contoursAreas0.begin() + (index++), std::pair<std::vector<cv::Point>, double>(c,area));
        }

        std::sort(contoursAreas0.rbegin(), contoursAreas0.rend(), [](const std::pair<std::vector<cv::Point>, double> &lhs,
                  const std::pair<std::vector<cv::Point>, double> &rhs )
        {
            return lhs.second < rhs.second;
        });


        std::vector<std::pair<std::vector<cv::Point>, double>> contoursAreas1;

        int num_countours = 1;
        if(contoursAreas0.size() > num_countours)
        {
            std::copy(contoursAreas0.begin(), contoursAreas0.begin() + num_countours, std::back_inserter(contoursAreas1));
        }
        else
        {
            std::copy(contoursAreas0.begin(), contoursAreas0.end(), std::back_inserter(contoursAreas1));
        }

        for(std::pair<std::vector<cv::Point>, double> &pa : contoursAreas1)
        {
            if(pa.first.size() > 0)
            {
                cv::RotatedRect bb = cv::minAreaRect(pa.first);

                cv::Rect initROI = bb.boundingRect();
                cv::Rect correctedROI;

                CVMisc::correctRect(valImg, initROI, correctedROI);

                std::pair<std::vector<cv::Point>, std::vector<cv::Point>> rect_pairs = CVMisc::extractSubRectPairs(bb);

                std::vector<cv::Point> convex0 = rect_pairs.first;
                std::vector<cv::Point> convex1 = rect_pairs.second;

                cv::RotatedRect rectROI0 = cv::minAreaRect(convex0);
                cv::RotatedRect rectROI1 = cv::minAreaRect(convex1);

                std::vector<cv::KeyPoint> keyPoints;
                std::vector<cv::KeyPoint> filteredKPs;

                brisk->detect(valImg, keyPoints);
                CVMisc::filterKeyPoints(bb, keyPoints, filteredKPs);

                std::vector<KeyPoint> kpOut0;
                std::vector<KeyPoint> kpOut1;


                std::vector<float> kpDensities0 = Features::keypointDensities(rectROI0, filteredKPs, kpOut0);
                std::vector<float> kpDensities1 = Features::keypointDensities(rectROI1, filteredKPs, kpOut1);

//                cv::drawKeypoints(outImg, filteredKPs, outImg, cv::Scalar(0,0,255));
                cv::drawKeypoints(outImg, keyPoints, outImg, cv::Scalar(0,0,255));

                float c0 = m_handSegArmClassifier.predict(kpDensities0);
                float c1 = m_handSegArmClassifier.predict(kpDensities1);


                cv::Rect2d rectROI;
                std::vector<KeyPoint> kpOut;
//                cv::Rect2d rectTrack;

//                if (m_trackerInit)
//                {
//                    tracker->update(hsvImg, m_trackRect);
//                    cv::rectangle(outImg, m_trackRect, cv::Scalar(50,150,255), 2);
//                }

//                qDebug() << "Rect State: (" << m_trackRect.tl().x << "," << m_trackRect.tl().y << ")" ;


                if (c0 == 1)
                {
                    rectROI = cv::Rect2d(rectROI0.boundingRect());
                    cv::Rect corrected;
                    CVMisc::correctRect(valImg, rectROI, corrected);
                    cv::rectangle(outImg, corrected, cv::Scalar(255,150,50));

                    kpOut = kpOut0;

                }

                if (c1 == 1)
                {
                    rectROI = cv::Rect2d(rectROI1.boundingRect());
                    cv::Rect corrected;
                    CVMisc::correctRect(valImg, rectROI, corrected);

                    cv::rectangle(outImg, corrected, cv::Scalar(255,150,50));

                    kpOut = kpOut1;
                }

                if (!m_trackerInit)
                {
                    m_trackRect = rectROI;
                    tracker->init(hsvImg, m_trackRect);
                    m_trackerInit = true;


                }

                tracker->update(hsvImg, m_trackRect);

                float areaI = CVMisc::rectIntersectArea(m_trackRect, rectROI);


                if (rectROI.area() != 0)
                {
                    float maxArea = std::max(m_trackRect.area(), rectROI.area());

                    // this is to avoid dividing by zero
                    if ((areaI/maxArea) < 0.3f)
                    {
                        m_trackRect = rectROI;
    //                    tracker->clear();
                        tracker = cv::Tracker::create("KCF");
                        tracker->init(hsvImg, m_trackRect);
                    }

                }

                if (m_trackRect.width != 0 && m_trackRect.height != 0)
                {
                    std::vector<float> subRectDensities = Features::keypointDensities(cv::Rect(m_trackRect), keyPoints, 3, 3);

//                    QString data;

                    QVector<float> myData = QVector<float>::fromStdVector(subRectDensities);



//                    for (const float & d : subRectDensities)
//                    {
//                        data.append(QString::number(d) + ",");
//                    }

//                    qDebug() << data;

                    qDebug() << myData;

//                    std::cout << "\n";
                }




                cv::rectangle(outImg, m_trackRect, cv::Scalar(50,50,50));


//                if ()
//                {
//                    m_trackerInit = tracker->init(hsvImg, cv::Rect2d(rectROI));


//                }






            }
        }



        if(dispMode == FILTER_NORMAL)
        {
            if(outImg.channels() == 3)
            {
                cv::cvtColor(outImg,outImg,CV_BGR2RGB);
                const uchar* data = const_cast<const uchar*>(outImg.data);
                qtImg = QImage(data,cols,rows, QImage::Format_RGB888);
            }
            else
            {
                const uchar* data = const_cast<const uchar*>(outImg.data);
                qtImg = QImage(data,cols,rows, QImage::Format_Indexed8);
            }

            auto flag0 = Qt::KeepAspectRatio;
            auto flag1 = Qt::SmoothTransformation;

            QPixmap pixMap = QPixmap::fromImage(qtImg).scaled(m_label_outImg->size(), flag0,flag1);

            m_label_outImg->setPixmap(pixMap);
        }
        else if (dispMode == FILTER_SKIN)
        {
            if(skinDetect.channels() == 3)
            {
                const uchar* data = const_cast<const uchar*>(skinDetect.data);
                qtImg = QImage(data,cols,rows, QImage::Format_RGB888);
            }
            else
            {
                const uchar* data = const_cast<const uchar*>(skinDetect.data);
                qtImg = QImage(data,cols,rows, QImage::Format_Indexed8);
            }

            auto flag0 = Qt::KeepAspectRatio;

            auto flag1 = Qt::SmoothTransformation;

            QPixmap pixMap = QPixmap::fromImage(qtImg).scaled(m_label_outImg->size(), flag0,flag1);

            m_label_outImg->setPixmap(pixMap);
        }


    }
}


void MainWindow::on_btn_PlayStop_clicked()
{
    if(m_camPlayer->isStopped())
    {
        m_camPlayer->initCam();
        m_camPlayer->play();
        ui->btn_PlayStop->setText(tr("Stop"));
    }
    else
    {
        m_camPlayer->stop();
        ui->btn_PlayStop->setText(tr("Play"));
    }
}

MainWindow::~MainWindow()
{
    delete m_camPlayer;
    delete ui;
}




void MainWindow::on_btn_capture_clicked()
{
    if(!m_camPlayer->isStopped())
    {
        m_camPlayer->stop();
        ui->btn_PlayStop->setText(tr("Play"));
    }

    m_imageCapture->showCapture(m_currMat);
}


void MainWindow::on_actionHand_Arm_Detector_toggled(bool toggled)
{

    qDebug() << "Arm Detector is Toggled!";
    m_handArmTrainer->show();
}


void MainWindow::on_m_handArmTrainer_has_closed()
{
    connect(m_camPlayer, SIGNAL(processCVMat(cv::Mat)), this, SLOT(on_receiveCVMat(cv::Mat)));
}

void MainWindow::on_m_handArmTrainer_has_shown()
{
    disconnect(m_camPlayer, SIGNAL(processCVMat(cv::Mat)), this, SLOT(on_receiveCVMat(cv::Mat)));
}







