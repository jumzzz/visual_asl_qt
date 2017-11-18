#include "handarmtrainer.h"
#include "ui_handarmtrainer.h"

HandSegArmTrainer::HandSegArmTrainer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HandArmTrainer)
{
    ui->setupUi(this);
    qRegisterMetaType<cv::Mat>("cv::Mat");

    m_label_outImg = new ClickLabel;

    ui->verticalLayout->addWidget(m_label_outImg);
    m_label_outImg->setAlignment(Qt::AlignCenter);
    m_label_outImg->setStyleSheet("Background-color: #000;");

    ui->cb_handPos->addItem(HAND_UP);
    ui->cb_handPos->addItem(HAND_DOWN);
    ui->cb_handPos->addItem(HAND_LEFT);
    ui->cb_handPos->addItem(HAND_RIGHT);

    ui->cb_logTest->addItem(NONE);
    ui->cb_logTest->addItem(LOG);
    ui->cb_logTest->addItem(TEST_DETECT);

    mainDirPath = QCoreApplication::applicationDirPath() + "/" + DIR_MAIN;
    subMainDirPath = mainDirPath + "/" + DIR_SUBMAIN;

    QDir mainDir(mainDirPath);
    QDir subMainDir(subMainDirPath);

    if (!mainDir.exists())
    {
        mainDir.mkdir(mainDirPath);
    }

    if (!subMainDir.exists())
    {
        subMainDir.mkdir(subMainDirPath);
    }


    filePathHandORBs = subMainDirPath + "/" + FILENAME_HAND_ORB;
    filePathNHandORBs = subMainDirPath + "/" + FILENAME_NHAND_ORB;

}

void HandSegArmTrainer::setHandSegArmClassifier(HandSegArmClassifier *handSegArmClassifier)
{
    m_handSegArmClassifier = handSegArmClassifier;
}

void HandSegArmTrainer::setCamplayer(CVCamPlayer *camPlayer)
{
    m_camPlayer = camPlayer;
}

void HandSegArmTrainer::setSkinDetector(SkinDetect *skinDetect)
{
    m_skinDetect = skinDetect;
}

HandSegArmTrainer::~HandSegArmTrainer()
{
    delete ui;
}

void HandSegArmTrainer::on_btn_PlayStop_clicked()
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

        logToFile(filePathHandORBs, m_ORBs_hand);
        logToFile(filePathNHandORBs, m_ORBs_nhand);
    }
}

void HandSegArmTrainer::closeEvent(QCloseEvent *event)
{
    qDebug() << "Hand Arm Trainer Dialog is closed.";

    if (m_camPlayer != NULL)
    {
        disconnect(m_camPlayer, SIGNAL(processCVMat(cv::Mat)), this, SLOT(on_receiveCVMat(cv::Mat)));

    }

    emit has_closed();
}

void HandSegArmTrainer::showEvent(QShowEvent *event)
{
    qDebug() << "Hand Arm Trainer Dialog is shown";

    if (m_camPlayer != NULL)
    {
        connect(m_camPlayer, SIGNAL(processCVMat(cv::Mat)), this, SLOT(on_receiveCVMat(cv::Mat)));
    }

    emit has_shown();
}

void HandSegArmTrainer::on_receiveCVMat(const cv::Mat &img)
{
    if(!img.empty())
    {
        int cols = img.cols;
        int rows = img.rows;

        QImage qtImg;

        m_currMat = img.clone();
        cv::Mat outImg = img.clone();
        cv::Mat hsvImg;
        cv::Mat grayImg;
        cv::cvtColor(img.clone(), hsvImg, CV_BGR2HSV_FULL);
        cv::cvtColor(img.clone(), grayImg, CV_BGR2GRAY);

        cv::Mat valImg;

        std::vector<cv::Mat> splitted;

        cv::split(hsvImg, splitted);

        valImg = splitted.at(2);

        cv::Mat skinDetect;

        m_skinDetect->filterSkin(hsvImg,skinDetect,HSV_FULL);

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

                CVMisc::correctRect(grayImg, initROI, correctedROI);

                std::pair<std::vector<cv::Point>, std::vector<cv::Point>> rect_pairs = CVMisc::extractSubRectPairs(bb);

                std::vector<cv::Point> hand_convex;
                std::vector<cv::Point> nhand_convex;

                assignArmHandPoints(rect_pairs.first, rect_pairs.second, hand_convex, nhand_convex);

                cv::Point handCentroid = CVMisc::centroid(hand_convex);
                cv::Point nhandCentroid = CVMisc::centroid(nhand_convex);

                cv::RotatedRect rectHand = cv::minAreaRect(hand_convex);
                cv::RotatedRect rectNHand = cv::minAreaRect(nhand_convex);

                std::vector<cv::KeyPoint> keyPoints;
                std::vector<cv::KeyPoint> filteredKPs;

                Features::ORBDescriptor(valImg,keyPoints);
                CVMisc::filterKeyPoints(bb, keyPoints, filteredKPs);


                std::vector<float> handORBDensities = Features::keypointDensities(rectHand, filteredKPs);
                std::vector<float> nhandORBDensities = Features::keypointDensities(rectNHand, filteredKPs);

                cv::drawKeypoints(outImg, filteredKPs, outImg, cv::Scalar(0,0,255));


                if (ui->cb_logTest->currentText() == TEST_DETECT && m_classifierLoaded)
                {
                    float c0 = m_handSegArmClassifier->predict(handORBDensities);
                    float c1 = m_handSegArmClassifier->predict(nhandORBDensities);

                    if (c1 == 1)
                    {
                        CVMisc::drawRotRect(outImg, rectHand, cv::Scalar(200,100,50), 1);
                        cv::putText(outImg, "Hand", handCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(255,0,0));
                    }

                    if (c0 == 1)
                    {

                        CVMisc::drawRotRect(outImg, rectNHand, cv::Scalar(200,100,50), 1);
                        cv::putText(outImg, "Hand", nhandCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0,0,255));
                    }
                }


                if (ui->cb_logTest->currentText() == NONE)
                {
                    CVMisc::drawRotRect(outImg, rectHand, cv::Scalar(0,0,255),1);
                    CVMisc::drawRotRect(outImg, rectNHand, cv::Scalar(255,0,0),1);

                    cv::putText(outImg, "Hand", handCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(255,0,0));
                    cv::putText(outImg, "Non-Hand", nhandCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0,0,255));
                }

                if (ui->cb_logTest->currentText() == LOG)
                {
                    CVMisc::drawRotRect(outImg, rectHand, cv::Scalar(0,0,255),1);
                    CVMisc::drawRotRect(outImg, rectNHand, cv::Scalar(255,0,0),1);

                    cv::putText(outImg, "Hand", handCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(255,0,0));
                    cv::putText(outImg, "Non-Hand", nhandCentroid, cv::FONT_HERSHEY_COMPLEX, 0.4, cv::Scalar(0,0,255));



                    if (handORBDensities.size() == nhandORBDensities.size())
                    {
                        QString csv_handORBs;
                        QString csv_nhandORBs;

                        for (int i = 0; i < handORBDensities.size(); i++)
                        {
                            csv_handORBs.append(QString::number(handORBDensities[i]));
                            csv_nhandORBs.append(QString::number(nhandORBDensities[i]));

                            csv_handORBs.append(",");
                            csv_nhandORBs.append(",");

                            if ( i == handORBDensities.size() - 1)
                            {
                                csv_handORBs.append("1");
                                csv_nhandORBs.append("0");

                                csv_handORBs.append("\n");
                                csv_nhandORBs.append("\n");

                            }
                        }


                        m_ORBs_hand.push_back(csv_handORBs);
                        m_ORBs_nhand.push_back(csv_nhandORBs);

                    }

                }
            }
        }


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
}

void HandSegArmTrainer::assignArmHandPoints(const std::vector<cv::Point> &pts0, const std::vector<cv::Point> &pts1, std::vector<cv::Point> &out_hand, std::vector<cv::Point> &out_nhand)
{

    cv::Point centroid0 = CVMisc::centroid(pts0);
    cv::Point centroid1 = CVMisc::centroid(pts1);

    if (ui->cb_handPos->currentText() == HAND_UP)
    {
        if (centroid0.y < centroid1.y)
        {
            out_hand.assign(pts0.begin(), pts0.end());
            out_nhand.assign(pts1.begin(), pts1.end());

        }
        else
        {
            out_hand.assign(pts1.begin(), pts1.end());
            out_nhand.assign(pts0.begin(), pts0.end());
        }
    }

    else if (ui->cb_handPos->currentText() == HAND_DOWN)
    {
        if (centroid0.y > centroid1.y)
        {
            out_hand.assign(pts0.begin(), pts0.end());
            out_nhand.assign(pts1.begin(), pts1.end());
        }
        else
        {
            out_hand.assign(pts1.begin(), pts1.end());
            out_nhand.assign(pts0.begin(), pts0.end());
        }
    }

    else if (ui->cb_handPos->currentText() == HAND_LEFT)
    {
        if (centroid0.x < centroid1.x)
        {
            out_hand.assign(pts0.begin(), pts0.end());
            out_nhand.assign(pts1.begin(), pts1.end());
        }
        else
        {
            out_hand.assign(pts1.begin(), pts1.end());
            out_nhand.assign(pts0.begin(), pts0.end());
        }
    }

    else if (ui->cb_handPos->currentText() == HAND_RIGHT)
    {
        if (centroid0.x < centroid1.x)
        {
            out_hand.assign(pts0.begin(), pts0.end());
            out_nhand.assign(pts1.begin(), pts1.end());
        }
        else
        {
            out_hand.assign(pts1.begin(), pts1.end());
            out_nhand.assign(pts0.begin(), pts0.end());
        }
    }

}


void HandSegArmTrainer::logToFile(const QString &path, QStringList &dataList)
{
    if (dataList.length() > 0)
    {
        QFile file(path);
        QTextStream fileStream(&file);

        file.open(QIODevice::WriteOnly | QIODevice::Append);

        if (file.isOpen())
        {
            for(const QString &lineData: dataList)
            {
                fileStream << lineData;
            }

            file.close();
        }

        dataList.clear();
    }
}

void HandSegArmTrainer::on_btn_trainData_clicked()
{
    QStringList filePaths;

    int numOfFeatures = 3;

    filePaths.append(filePathHandORBs);
    filePaths.append(filePathNHandORBs);

    std::vector<float> weights(numOfFeatures, 1.0f);

    m_handSegArmClassifier->loadCSVData(filePaths, numOfFeatures, weights);

    qDebug() << "Hand to Arm Training Ends..";

    m_classifierLoaded = true;
}

void HandSegArmTrainer::on_btn_loadData_clicked()
{
//    m_handSegArmClassifier->loadTrainedData();
    m_handSegArmClassifier->loadTrainedData();
    m_classifierLoaded = true;

}
