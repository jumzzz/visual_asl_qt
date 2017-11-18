#ifndef CLASSIFIERMANAGER_H
#define CLASSIFIERMANAGER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/ml.hpp"

#include <QFile>

#include <QCoreApplication>

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>

#include <vector>
#include <algorithm>
#include <string>



class HandSegArmClassifier
{
public:
    explicit HandSegArmClassifier();

    void init();
    void loadCSVData(const QStringList &path, int featureSize, std::vector<float> &weights);
    void loadTrainedData();
//    void loadTrainedData();

    void trainData(cv::Mat &featureVectors, cv::Mat &featureLabels, int numOfLayers, int numOfNodes);
    void trainSVMData(cv::Mat &featureVectors, cv::Mat &featureLabels);

    float predict(double input[], int size, std::vector<float> weights);
    float predict(std::vector<float> &input);


    float predictSVM(double input[], int size, std::vector<float> weights);
    float predictSVM(std::vector<float> &input);


    bool isTrained();


private:

    const QString DIR_MAIN = "Train";
    const QString DIR_SUBMAIN = "Arm_Hand_Seg";
    const QString FILENAME_ARMHANDSEG = "arm_handseg.xml";

    QString m_trainingFilePath;

    cv::Ptr<cv::ml::ANN_MLP> m_annClassifier;
    cv::Ptr<cv::ml::SVM> m_svmClassifier;

    cv::Mat m_handLabel;
    cv::Mat m_nhandLabel;

    const int NHAND_LABEL = 0;
    const int HAND_LABEL = 1;


//    std::vector<int> m_layers;

};


class ClassifierManager
{
public:
    explicit ClassifierManager();


private:


};

#endif // CLASSIFIERMANAGER_H
