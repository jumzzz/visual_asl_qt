#ifndef HANDARMTRAINER_H
#define HANDARMTRAINER_H

#include <QDialog>

#include "core_header.h"

#include <QCloseEvent>
#include <QShowEvent>

#include "classifiermanager.h"
#include "featureextract.h"

namespace Ui {
class HandArmTrainer;
}

class HandSegArmTrainer : public QDialog
{
    Q_OBJECT

public:
    explicit HandSegArmTrainer(QWidget *parent = 0);
    ~HandSegArmTrainer();

    void setCamplayer(CVCamPlayer *camPlayer);
    void setSkinDetector(SkinDetect *skinDetect);
    void setHandSegArmClassifier(HandSegArmClassifier *handSegArmClassifier);


signals:
    void has_shown();
    void has_closed();

private slots:
    void on_receiveCVMat(const cv::Mat &img);
    void on_btn_PlayStop_clicked();
    void on_btn_trainData_clicked();
    void on_btn_loadData_clicked();

private:
    Ui::HandArmTrainer *ui;

    ClickLabel *m_label_outImg;

    cv::Mat m_currMat;

    CVCamPlayer *m_camPlayer;
    SkinDetect *m_skinDetect;

    const QString HAND_UP = "Hand Up";
    const QString HAND_DOWN = "Hand Down";
    const QString HAND_LEFT = "Hand Left";
    const QString HAND_RIGHT = "Hand Right";

    const QString NONE = "None";
    const QString LOG = "Log";
    const QString TEST_DETECT = "Test Detection";


    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void assignArmHandPoints(const std::vector<cv::Point> &pts0, const std::vector<cv::Point> &pts1, std::vector<cv::Point> &out_hand, std::vector<cv::Point> &out_nhand);
    void logToFile(const QString &path, QStringList &dataList);


    QStringList m_orbDensities_hand;
    QStringList m_orbDensities_nhand;

    QStringList m_ORBs_hand;
    QStringList m_ORBs_nhand;


    const QString DIR_MAIN = "Train";
    const QString DIR_SUBMAIN = "Arm_Hand_Seg";

    const QString FILENAME_HAND_ORB = "handseg_orb.csv";
    const QString FILENAME_NHAND_ORB = "nhandseg_orb.csv";

    QString filePathHandORBs;
    QString filePathNHandORBs;

    QString mainDirPath;
    QString subMainDirPath;

    HandSegArmClassifier *m_handSegArmClassifier;

    bool m_classifierLoaded = false;

    std::vector<float> m_weights;
};

#endif // HANDARMTRAINER_H
