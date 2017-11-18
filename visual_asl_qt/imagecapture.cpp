#include "imagecapture.h"
#include "ui_imagecapture.h"

ImageCapture::ImageCapture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageCapture)
{
    ui->setupUi(this);

    m_data_man = new DataManager;
    label_outImg = new ClickLabel;

    ui->verticalLayout->addWidget(label_outImg);
    label_outImg->setAlignment(Qt::AlignCenter);
    label_outImg->setStyleSheet("Background-color: #000;");

    connect(label_outImg, SIGNAL(mousePressed(QPoint)), this, SLOT(on_label_outImg_mousePressed(QPoint)));
    connect(label_outImg, SIGNAL(mouseReleased(QPoint)), this, SLOT(on_label_outImg_mouseReleased(QPoint)));

    ui->cb_colorSpace->addItem(KEY_RGB);
    ui->cb_colorSpace->addItem(KEY_BGR);
    ui->cb_colorSpace->addItem(KEY_HSV);
    ui->cb_colorSpace->addItem(KEY_HSV_FULL);
    ui->cb_colorSpace->addItem(KEY_LAB);
    ui->cb_colorSpace->addItem(KEY_LUV);

    ui->cb_trainMode->addItem(TRAIN_SKIN);
    ui->cb_trainMode->addItem(TRAIN_HANDSEG);

    m_swIndex_map.insert(std::pair<QString, int>(TRAIN_SKIN,0));
    m_swIndex_map.insert(std::pair<QString, int>(TRAIN_HANDSEG,1));

//    ui->sw_controls->setCurrentIndex(m_swIndex_map.at(TRAIN_SKIN));
//    ui->sw_out->setCurrentIndex(m_swIndex_map.at(TRAIN_SKIN));


    ui->sw_controls->setCurrentIndex(m_swIndex_map.at(TRAIN_SKIN));
    ui->sw_out->setCurrentIndex(m_swIndex_map.at(TRAIN_SKIN));

    std::vector<cv::Mat> initTmp;

    m_skin_map[KEY_RGB] = initTmp;
    m_skin_map[KEY_BGR] = initTmp;
    m_skin_map[KEY_HSV] = initTmp;
    m_skin_map[KEY_HSV_FULL] = initTmp;
    m_skin_map[KEY_LAB] = initTmp;
    m_skin_map[KEY_LUV] = initTmp;

    m_nskin_map[KEY_RGB] = initTmp;
    m_nskin_map[KEY_BGR] = initTmp;
    m_nskin_map[KEY_HSV] = initTmp;
    m_nskin_map[KEY_HSV_FULL] = initTmp;
    m_nskin_map[KEY_LAB] = initTmp;
    m_nskin_map[KEY_LUV] = initTmp;


    m_handROIColor = cv::Scalar(0,255,0);
    m_bgROIColor = cv::Scalar(0,0,255);



}

void ImageCapture::setSkinDetector(SkinDetect *skinDetect)
{
    m_skinDetect = skinDetect;
}

bool ImageCapture::checkROIValidity(const cv::Point &p0, const cv::Point &p1)
{
    float mat_w = (float)m_currMat.cols;
    float mat_h = (float)m_currMat.rows;

    float tw = (float)label_outImg->width();
    float th = (float)label_outImg->height();

    float aw = (float)label_outImg->pixmap()->width();
    float ah = (float)label_outImg->pixmap()->height();

    float oax = (tw - aw)/2.0f;
    float oay = (th - ah)/2.0f;

    float delX0 = (float)p0.x - oax;
    float delY0 = (float)p0.y - oay;

    float delX1 = (float)p1.x - oax;
    float delY1 = (float)p1.y - oay;

//    float delX1 = (float)loc.x() - oax;
//    float delY1 = (float)loc.y() - oay;


    // check deltas validity

    bool vx0 = delX0 > 0.0f && delX0 < mat_w;
    bool vy0 = delY0 > 0.0f && delY0 < mat_h;

    bool vx1 = delX1 > 0.0f && delX1 < mat_w;
    bool vy1 = delY1 > 0.0f && delY1 < mat_h;

    bool deltaValid = vx0 && vy0 && vx1 && vy1;

    return deltaValid;
}

cv::Rect ImageCapture::generateROI(const cv::Point &p0, const cv::Point &p1)
{
    float mat_w = (float)m_currMat.cols;
    float mat_h = (float)m_currMat.rows;

    float tw = (float)label_outImg->width();
    float th = (float)label_outImg->height();

    float aw = (float)label_outImg->pixmap()->width();
    float ah = (float)label_outImg->pixmap()->height();

    float oax = (tw - aw)/2.0f;
    float oay = (th - ah)/2.0f;

    float delX0 = (float)p0.x - oax;
    float delY0 = (float)p0.y - oay;

    float delX1 = (float)p1.x - oax;
    float delY1 = (float)p1.y - oay;

//    float delX1 = (float)loc.x() - oax;
//    float delY1 = (float)loc.y() - oay;


    // check deltas validity

    bool vx0 = delX0 > 0.0f && delX0 < mat_w;
    bool vy0 = delY0 > 0.0f && delY0 < mat_h;

    bool vx1 = delX1 > 0.0f && delX1 < mat_w;
    bool vy1 = delY1 > 0.0f && delY1 < mat_h;

    bool deltaValid = vx0 && vy0 && vx1 && vy1;
    qDebug() << "Delta Valid: " << deltaValid;

    if (deltaValid)
    {
        float ux0 = delX0/aw;
        float uy0 = delY0/ah;

        float ux1 = delX1/aw;
        float uy1 = delY1/ah;

        int px0 = (int)(ux0*mat_w);
        int py0 = (int)(uy0*mat_h);

        int px1 = (int)(ux1*mat_w);
        int py1 = (int)(uy1*mat_h);


        cv::Point pt0(px0,py0);
        cv::Point pt1(px1,py1);

        GeometryCompute gc;

        cv::Rect crop_rect = gc.genCropRect(pt0,pt1);

        return crop_rect;
    }
    else
        return cv::Rect();
}

void ImageCapture::skin_mousePressed_Action(const QPoint &loc)
{
    m_cvPt0 = cv::Point(loc.x(),loc.y());
}

void ImageCapture::skin_mouseReleased_Action(const QPoint &loc)
{
    m_cvPt1 = cv::Point(loc.x(), loc.y());

    cv::Rect crop_rect = generateROI(m_cvPt0,m_cvPt1);

    if(crop_rect != cv::Rect())
    {
        cv::Mat drawImg = m_currMat.clone();
        m_currMat.copyTo(drawImg);

        cv::rectangle(drawImg, crop_rect, cv::Scalar(255,0,0),2);
        cv::namedWindow("Selected Crop", cv::WINDOW_AUTOSIZE);
        cv::imshow("Selected Crop", drawImg);

        MatConverter mcvt(m_currMat.clone());

        cv::Mat tbcBGR = mcvt.getBGR().getMat(cv::ACCESS_FAST);
        cv::Mat tbcRGB = mcvt.getRGB().getMat(cv::ACCESS_FAST);
        cv::Mat tbcHSV = mcvt.getHSV().getMat(cv::ACCESS_FAST);
        cv::Mat tbcHSV_FULL = mcvt.getHSVFull().getMat(cv::ACCESS_FAST);
//        cv::equalizeHist(tbcHSV_FULL,tbcHSV_FULL);
        cv::Mat tbcLAB = mcvt.getLab().getMat(cv::ACCESS_FAST);
        cv::Mat tbcLUV = mcvt.getLUV().getMat(cv::ACCESS_FAST);

        cv::Mat cropped_bgr(tbcBGR, crop_rect);
        cv::Mat cropped_rgb(tbcRGB, crop_rect);
        cv::Mat cropped_hsv(tbcHSV, crop_rect);
        cv::Mat cropped_hsvFull(tbcHSV_FULL, crop_rect);
        cv::Mat cropped_lab(tbcLAB, crop_rect);
        cv::Mat cropped_luv(tbcLUV, crop_rect);


        if (!cropped_bgr.isContinuous())
            cropped_bgr = cropped_bgr.clone();

        if (!cropped_rgb.isContinuous())
            cropped_rgb = cropped_rgb.clone();

        if (!cropped_hsv.isContinuous())
            cropped_hsv = cropped_hsv.clone();

        if (!cropped_hsvFull.isContinuous())
            cropped_hsvFull = cropped_hsvFull.clone();

        if (!cropped_lab.isContinuous())
            cropped_lab = cropped_lab.clone();

        if (!cropped_luv.isContinuous())
            cropped_luv = cropped_luv.clone();


        if(ui->rb_skin->isChecked())
        {
            m_skin_map.at(KEY_BGR).push_back(cropped_bgr);
            m_skin_map.at(KEY_RGB).push_back(cropped_rgb);
            m_skin_map.at(KEY_HSV).push_back(cropped_hsv);
            m_skin_map.at(KEY_HSV_FULL).push_back(cropped_hsvFull);
            m_skin_map.at(KEY_LAB).push_back(cropped_lab);
            m_skin_map.at(KEY_LUV).push_back(cropped_luv);

            QString skinNum = QString::number(m_skin_map.at(KEY_BGR).size());
            ui->label_skinCount->setText(skinNum);
        }
        else
        {
            m_nskin_map.at(KEY_BGR).push_back(cropped_bgr);
            m_nskin_map.at(KEY_RGB).push_back(cropped_rgb);
            m_nskin_map.at(KEY_HSV).push_back(cropped_hsv);
            m_nskin_map.at(KEY_HSV_FULL).push_back(cropped_hsvFull);
            m_nskin_map.at(KEY_LAB).push_back(cropped_lab);
            m_nskin_map.at(KEY_LUV).push_back(cropped_luv);

            QString skinNum = QString::number(m_nskin_map.at(KEY_BGR).size());
            ui->label_nSkinCount->setText(skinNum);

        }

    }


}

void ImageCapture::handSeg_mousePressed_Action(const QPoint &loc)
{
    m_cvPt0 = cv::Point(loc.x(),loc.y());
}

void ImageCapture::handSeg_mouseReleased_Action(const QPoint &loc)
{
    m_cvPt1 = cv::Point(loc.x(), loc.y());

    cv::Rect crop_rect = generateROI(m_cvPt0,m_cvPt1);

    if(crop_rect != cv::Rect())
    {
        cv::Mat input_img;

        m_currMat.copyTo(input_img);


        if(ui->rb_hand->isChecked())
        {
            handROI.push_back(crop_rect);
        }
        else if (ui->rb_background->isChecked())
        {
            bgROI.push_back(crop_rect);
        }


        for(cv::Rect &rect : handROI)
        {
            cv::rectangle(input_img,rect,m_handROIColor,1);
        }

        for(cv::Rect &rect : bgROI)
        {
            cv::rectangle(input_img,rect,m_bgROIColor,1);
        }


        m_setImgMatrix(input_img);

    }
}

void ImageCapture::on_label_outImg_mousePressed(const QPoint &loc)
{
    if(ui->cb_trainMode->currentText() == TRAIN_SKIN)
        skin_mousePressed_Action(loc);

    else if (ui->cb_trainMode->currentText() == TRAIN_HANDSEG)
        handSeg_mousePressed_Action(loc);
}

void ImageCapture::on_label_outImg_mouseReleased(const QPoint &loc)
{
    if(ui->cb_trainMode->currentText() == TRAIN_SKIN)
        skin_mouseReleased_Action(loc);

    else if (ui->cb_trainMode->currentText() == TRAIN_HANDSEG)
        handSeg_mouseReleased_Action(loc);
}


void ImageCapture::m_setCurrMat(const cv::Mat &img)
{
    m_currMat = img.clone();
}

void ImageCapture::m_setImgMatrix(const cv::Mat &img)
{
    if(!img.empty())
    {
//        m_currMat = img.clone();



        int cols = img.cols;
        int rows = img.rows;

        QImage qtImg;

        MatConverter mcvt(img);
        cv::Mat outImg;

        QString selectedCS = ui->cb_colorSpace->currentText();


        if(selectedCS == KEY_BGR)
        {
            outImg = mcvt.getBGR().getMat(cv::ACCESS_FAST);
        }
        else if (selectedCS == KEY_HSV)
        {
            outImg = mcvt.getHSV().getMat(cv::ACCESS_FAST);
        }
        else if (selectedCS == KEY_HSV_FULL)
        {
            outImg = mcvt.getHSVFull().getMat(cv::ACCESS_FAST);
//            cv::equalizeHist(outImg,outImg);
        }
        else if (selectedCS == KEY_LAB)
        {
            outImg = mcvt.getLab().getMat(cv::ACCESS_FAST);
        }
        else if (selectedCS == KEY_LUV)
        {
            outImg = mcvt.getLUV().getMat(cv::ACCESS_FAST);
        }
        else
        {
            outImg = mcvt.getRGB().getMat(cv::ACCESS_FAST);
        }


        if(outImg.channels() == 3)
        {
            const uchar* data = const_cast<const uchar*>(outImg.data);
            qtImg = QImage(data,cols,rows, QImage::Format_RGB888);
        }
        else
        {
           const uchar* data = const_cast<const uchar*>(outImg.data);
           qtImg = QImage(data,cols,rows, QImage::Format_Indexed8);
        }

        auto flag0 = Qt::KeepAspectRatio;
        auto flag1 = Qt::FastTransformation;

//        QPixmap pixMap = QPixmap::fromImage(qtImg).scaled(label_outImg->size(), flag0,flag1);

        QPixmap pixMap = QPixmap::fromImage(qtImg);

        label_outImg->setPixmap(pixMap);

    }

}

void ImageCapture::showCapture(const cv::Mat &img)
{
    m_setCurrMat(img);
    m_setImgMatrix(img);
    this->show();
}

void ImageCapture::setFileManager(FileManager *fileManager)
{
    m_fileManager = fileManager;
}



cv::Vec3b ImageCapture::m_averageIntensity(const cv::Mat &img)
{

    uint c0 = 0;
    uint c1 = 0;
    uint c2 = 0;

    const int channels = img.channels();

    switch(channels)
    {
    case 1:
        {
           cv::MatConstIterator_<uchar> it,end;
           for( it = img.begin<uchar>(), end = img.end<uchar>(); it != end; ++it)
                c0 += *it;
            break;
        }
    case 3:
        {
            cv::MatConstIterator_<cv::Vec3b> it, end;
            for( it = img.begin<cv::Vec3b>(), end = img.end<cv::Vec3b>(); it != end; ++it)
            {
                c0 += (uint)((*it)[0]);
                c1 += (uint)((*it)[1]);
                c2 += (uint)((*it)[2]);
            }
        }
    }

    int n = img.rows*img.cols;

    uint ave0 = c0/n;
    uint ave1 = c1/n;
    uint ave2 = c2/n;

    cv::Vec3b vec(ave0,ave1,ave2);

    return vec;
}

ImageCapture::~ImageCapture()
{
    delete ui;
}

void ImageCapture::on_btn_addSamples_clicked()
{

}

void ImageCapture::on_btn_undoSamples_clicked()
{
//    if(ui->rb_skin->isChecked())
//    {
//        m_skinCrop.pop();
//        QString skinNum = QString::number(m_skinCrop.size());
//        ui->label_skinCount->setText(skinNum);
//    }
//    else
//    {
//        m_nSkinCrop.pop();
//        QString nSkinNum = QString::number(m_nSkinCrop.size());
//        ui->label_nSkinCount->setText(nSkinNum);
//    }
}

void ImageCapture::m_clearData()
{
    for(auto &kv : m_skin_map)
    {
        kv.second.clear();
    }

    for(auto &kv : m_nskin_map)
    {
        kv.second.clear();
    }
}

void ImageCapture::on_btn_save_clicked()
{
    qDebug() << "Skin BGR map size: " << m_skin_map.at(KEY_BGR).size();
    qDebug() << "Skin RGB Skin map size: " << m_skin_map.at(KEY_RGB).size();
    qDebug() << "Skin HSV map size: " << m_skin_map.at(KEY_HSV).size();
    qDebug() << "Skin HSV FULL map size: " << m_skin_map.at(KEY_HSV_FULL).size();
    qDebug() << "Skin LAB map size: " << m_skin_map.at(KEY_LAB).size();
    qDebug() << "Skin LUV map size: " << m_skin_map.at(KEY_LUV).size();


    qDebug() << "Non-Skin BGR map size: " << m_nskin_map.at(KEY_BGR).size();
    qDebug() << "Non-Skin RGB Skin map size: " << m_nskin_map.at(KEY_RGB).size();
    qDebug() << "Non-Skin HSV map size: " << m_nskin_map.at(KEY_HSV).size();
    qDebug() << "Non-Skin HSV FULL map size: " << m_nskin_map.at(KEY_HSV_FULL).size();
    qDebug() << "Non-Skin LAB map size: " << m_nskin_map.at(KEY_LAB).size();
    qDebug() << "Non-Skin LUV map size: " << m_nskin_map.at(KEY_LUV).size();



     m_fileManager->saveSkinRawData(m_skin_map, true);
     m_fileManager->saveSkinRawData(m_nskin_map, false);

     m_clearData();

     QString skinNum = QString::number(m_skin_map.at(KEY_BGR).size());
     ui->label_skinCount->setText(skinNum);

     QString nSkinNum = QString::number(m_nskin_map.at(KEY_BGR).size());
     ui->label_nSkinCount->setText(nSkinNum);
}



void ImageCapture::on_btn_cancel_clicked()
{
//     m_skinCrop = std::stack<cv::Mat>();
//     m_nSkinCrop = std::stack<cv::Mat>();

//     QString skinNum = QString::number(m_skinCrop.size());
//     ui->label_skinCount->setText(skinNum);

//     QString nSkinNum = QString::number(m_nSkinCrop.size());
//     ui->label_nSkinCount->setText(nSkinNum);

    m_clearData();

    QString skinNum = QString::number(m_skin_map.at(KEY_BGR).size());
    ui->label_skinCount->setText(skinNum);

    QString nSkinNum = QString::number(m_nskin_map.at(KEY_BGR).size());
    ui->label_nSkinCount->setText(nSkinNum);

     this->hide();
}

void ImageCapture::on_cb_colorSpace_currentIndexChanged(const QString &arg1)
{
    m_setImgMatrix(m_currMat);
}

void ImageCapture::on_cb_trainMode_currentIndexChanged(const QString &arg1)
{
//    if(m_swIndex_map.find())
    if(m_swIndex_map.find(arg1) != m_swIndex_map.end())
    {
        int index = m_swIndex_map.at(arg1);

        ui->sw_controls->setCurrentIndex(index);
        ui->sw_out->setCurrentIndex(index);
    }
}

void ImageCapture::on_cb_colorSpace_activated(const QString &arg1)
{

}

void ImageCapture::on_btn_saveHandSeg_clicked()
{
    std::vector<cv::Mat> handImg(handROI.size());
    std::vector<cv::Mat> bgImg(bgROI.size());

//    std::vector<cv::Mat> handImg_laplacian(handROI.size());
//    std::vector<cv::Mat> bgImg_laplacian(bgROI.size());

//    std::vector<cv::Mat> handImg_sobel(handROI.size());
//    std::vector<cv::Mat> bgImg_sobel(bgROI.size());

//    std::vector<cv::Mat> handImg_canny(handROI.size());
//    std::vector<cv::Mat> bgImg_canny(bgROI.size());


    cv::Mat parentImg;

    m_currMat.copyTo(parentImg);


    cv::namedWindow("Sample Image");
    cv::imshow("Sample Image", parentImg);

    cv::Mat parentHSV;

    cv::cvtColor(parentImg,parentHSV,CV_BGR2HSV_FULL);

    std::vector<cv::Mat> hsv_channels(parentHSV.channels());

    cv::split(parentHSV, hsv_channels);

//    cv::Mat hue = hsv_channels.at(0);
//    cv::Mat sat = hsv_channels.at(1);
    cv::Mat val = hsv_channels.at(2);

    int handIndex = 0;
    int bgIndex = 0;

    for(cv::Rect &rect : handROI)
    {
        cv::Mat imgTmp(val, rect);
        imgTmp = imgTmp.clone();

        handImg.insert(handImg.begin() + handIndex++, imgTmp);
    }

    for(cv::Rect &rect : bgROI)
    {
        cv::Mat imgTmp(val, rect);
        imgTmp = imgTmp.clone();

        bgImg.insert(bgImg.begin() + bgIndex++, imgTmp);
    }


    QDir handGSDir(m_fileManager->get_dirPathSegmentHandGS());
    long nf_handGS = handGSDir.entryInfoList().length() + 1;


    for(cv::Mat &img : handImg)
    {
        cv::Mat writeImg = img.clone();

        if(!writeImg.empty())
        {
            QString absFileName = QString::number(nf_handGS++) + ".png";

            QString filePath = handGSDir.absolutePath() + "/" + absFileName;
            std::string filePathSTD = filePath.toStdString();

            cv::String filePathCV(filePathSTD);
            cv::imwrite(filePathCV,writeImg);
        }

    }

    QDir bgGSDir(m_fileManager->get_dirPathSegmentBGGS());
    long nf_bgGS = bgGSDir.entryInfoList().length() + 1;


    for(cv::Mat &img : bgImg)
    {
        cv::Mat writeImg = img.clone();

        if(!writeImg.empty())
        {
            QString absFileName = QString::number(nf_bgGS++) + ".png";

            QString filePath = bgGSDir.absolutePath() + "/" + absFileName;
            std::string filePathSTD = filePath.toStdString();

            cv::String filePathCV(filePathSTD);
            cv::imwrite(filePathCV,writeImg);
        }
    }


    handROI.clear();
    bgROI.clear();



}

void ImageCapture::on_btn_Train_clicked()
{

    QString posDir = m_fileManager->get_dirPathSegmentHandGS() + "*.png";
    QString negDir = m_fileManager->get_dirPathSegmentBGGS() + "*.png";



    QString filePath = QCoreApplication::applicationDirPath() + "/" + FILE_WBDETECT;

    QFile file(filePath);

    if(!file.exists())
    {
//        QTextStream io(file,QIODevice::WriteOnly);
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    std::string fileInputStd = filePath.toStdString();

    cv::String fileInputCV(fileInputStd);
    cv::FileStorage fs(fileInputCV,cv::FileStorage::WRITE);





}



void ImageCapture::on_btn_deleteAll_clicked()
{

      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Delete All Files", "Are you sure you want to delete all skin data?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes)
      {
        m_fileManager->deleteAllSkinFiles();
      }
}
