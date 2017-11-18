#include "classifiermanager.h"


HandSegArmClassifier::HandSegArmClassifier()
{

}


void HandSegArmClassifier::init()
{

    // Problem
    // Knowing that DIR_MAIN and DIR_SUBMAIN will be auto generated
    // by handarmtrainer dialog... how to make sure that m_trainingFilePath
    // will be generated???

    // Solution: Instantiate HandtoArmClassifier after the code that
    // generates DIR_MAIN and DIR SUBMAIN was generated

    QString mainAppDir = QCoreApplication::applicationDirPath();
    QString trainingFolder = mainAppDir + "/" + DIR_MAIN + "/" + DIR_SUBMAIN;
    m_trainingFilePath = trainingFolder + "/" + FILENAME_ARMHANDSEG;


    m_handLabel = cv::Mat(1,2, CV_32FC1);
    m_nhandLabel = cv::Mat(1,2, CV_32FC1);

    m_handLabel.at<float>(0, 0) = 1;
    m_handLabel.at<float>(0, 1) = 0;

    m_nhandLabel.at<float>(0, 0) = 0;
    m_nhandLabel.at<float>(0, 1) = 1;

}



void HandSegArmClassifier::loadCSVData(const QStringList &pathList, int featureSize, std::vector<float> &weights)
{

    std::vector<std::vector<float>> featureVectorList;
    std::vector<float> labelList;

    QStringList rawDatas;

    foreach(const QString &path, pathList)
    {
        QFile inputFile(path);

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);

            while(!in.atEnd())
            {
                QString lineData = in.readLine();
                rawDatas.push_back(lineData);

            }

            inputFile.close();
        }
    }

    std::random_shuffle(rawDatas.begin(), rawDatas.end());

    foreach(const QString &str, rawDatas)
    {


        QString lineData = str;

        QStringList lineDataValues = lineData.split(",");

        if (lineDataValues.count() == featureSize + 1)
        {
            std::vector<float> featureVector;

            for(int i = 0; i < featureSize; i++)
            {
                bool featureParsed = true;
                float value = lineDataValues.at(i).toFloat(&featureParsed);


                if (featureParsed)
                {
                    featureVector.push_back(value);
                }
            }

            bool labelParsed = true;
            float label = lineDataValues.at(featureSize).toFloat(&labelParsed);

            qDebug() << "Label passed: " << label;

            bool isDimJustRight = featureVector.size() == featureSize;
            bool isWeightJustRight  = weights.size() == featureSize;


            if (isDimJustRight && labelParsed && isWeightJustRight)
            {
                std::vector<float> finalFeature;

                for(int i = 0; i < featureSize; i++)
                {
                    float finalValue = featureVector.at(i)*weights.at(i);
                    finalFeature.push_back(finalValue);
                }

                featureVectorList.push_back(finalFeature);
                labelList.push_back(label);
            }

        }
    }

    if (featureVectorList.size() == labelList.size())
    {
        cv::Mat featureVectors(featureVectorList.size(), featureSize, CV_32FC1);
        cv::Mat featureLabels(featureVectorList.size(), 2, CV_32FC1);


        for(int i = 0; i < featureVectors.rows; i++)
        {
            for (int j = 0; j < featureVectors.cols; j++)
            {
                featureVectors.at<float>(i,j) = featureVectorList.at(i).at(j);
            }
        }

        for(int i = 0; i < featureLabels.rows; i++)
        {
            if(labelList.at(i) == 0)
            {
                featureLabels.at<float>(i,0) = 1;
                featureLabels.at<float>(i,1) = 0;
            }
            else
            {
                featureLabels.at<float>(i,0) = 0;
                featureLabels.at<float>(i,1) = 1;
            }

        }

        qDebug() << "Feature Vector Size: " << featureVectors.rows << "," << featureVectors.cols;
        qDebug() << "Feature Label Size: " << featureLabels.rows << "," << featureLabels.cols;

        trainData(featureVectors,featureLabels, 10, featureSize);
//        trainSVMData(featureVectors, featureLabels);
    }
}




bool HandSegArmClassifier::isTrained()
{
    return m_annClassifier->isTrained();
}

void HandSegArmClassifier::trainSVMData(cv::Mat &featureVectors, cv::Mat &featureLabels)
{
    // Set up SVM's parameters
//    cv::ml::SVM::Params params;
//    params.svmType    = cv::ml::SVM::C_SVC;
//    params.kernelType = cv::ml::SVM::LINEAR;
//    params.termCrit   = cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6);

    qDebug() << "Features Size: " << featureVectors.rows << "," << featureVectors.cols
              << " Label Size: " <<  featureLabels.rows <<","<< featureLabels.cols;

    m_svmClassifier = cv::ml::SVM::create();


    m_svmClassifier->setType(cv::ml::SVM::C_SVC);
    m_svmClassifier->setKernel(cv::ml::SVM::INTER);
    m_svmClassifier->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 10000, 1e-10));

    qDebug() << "Training SVM Starts";
    m_svmClassifier->train(featureVectors,cv::ml::ROW_SAMPLE, featureLabels);

    qDebug() << "Training SVM Ends";
//    qDebug() << "SVM Num of Iter: " << m_svmClassifier->;


    if (m_svmClassifier->isTrained())
    {
        std::string trainPathStd(m_trainingFilePath.toStdString());
        cv::String trainPathCV(trainPathStd);

        m_svmClassifier->save(trainPathCV);
    }

}

void HandSegArmClassifier::trainData(cv::Mat &featureVectors, cv::Mat &featureLabels, int numOfLayers, int numOfNodes)
{
//    qDebug() << "Feature Size: " << m_featureVectorList.size();
//    qDebug() << "Label Size: " << m_labelList.size();
    //    m_kClassfier = cv::ml::KNearest::create();


    int networkInputSize = featureVectors.cols;
    int networkOutputSize = featureLabels.cols;

    std::vector<int> layerSizes;

    layerSizes.push_back(networkInputSize);

    for (int i = 0; i < numOfLayers; i++)
    {
       layerSizes.push_back(numOfNodes);
    }

    layerSizes.push_back(networkOutputSize);

    m_annClassifier = cv::ml::ANN_MLP::create();
    m_annClassifier->setLayerSizes(layerSizes);
    m_annClassifier->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);

    qDebug() << "Training ANN:";
    m_annClassifier->train(featureVectors, cv::ml::ROW_SAMPLE, featureLabels);
    qDebug() << "ANN Trained!";

    if (m_annClassifier->isTrained())
    {
        std::string trainPathStd(m_trainingFilePath.toStdString());
        cv::String trainPathCV(trainPathStd);

        m_annClassifier->save(trainPathCV);
    }

//    m_featureLabelSet.release();
//    m_featureVectorsSet.release();
//    m_featureVectorList.clear();
//    m_labelList.clear();
}

float HandSegArmClassifier::predict(std::vector<float> &input)
{
    cv::Mat inputArray(1,input.size(), CV_32FC1);

    for(int j = 0; j < input.size(); j++)
    {
        inputArray.at<float>(0,j) = input.at(j);
    }

    cv::Mat outputArray;
    float result = m_annClassifier->predict(inputArray,outputArray);

    return result;
}

float HandSegArmClassifier::predictSVM(std::vector<float> &input)
{
    cv::Mat inputArray(1,input.size(), CV_32FC1);

    for(int j = 0; j < input.size(); j++)
    {
        inputArray.at<float>(0,j) = input.at(j);
    }

    cv::Mat outputArray;
    float result = m_svmClassifier->predict(inputArray);

    return result;

}



float HandSegArmClassifier::predict(double input[], int size, std::vector<float> weights)
{
    qDebug() << "Predict Size: " << size;

    std::vector<float> inputVector;

    for(int i = 0; i < size; i++)
    {
        inputVector.insert(inputVector.begin() + i, (float)input[i]);
    }

    std::vector<float> finalVector;

    for(int i = 0 ; i < size; i++)
    {
        finalVector.push_back(weights.at(i)*inputVector.at(i));
    }

    qDebug() << "Input Vector Size: " << inputVector.size();



    return predict(finalVector);
}


void HandSegArmClassifier::loadTrainedData()
{
    if (QFile(m_trainingFilePath).exists())
    {
        std::string trainPathStd(m_trainingFilePath.toStdString());
        cv::String trainPathCV(trainPathStd);

        m_annClassifier = cv::ml::ANN_MLP::create();
        m_annClassifier = cv::ml::ANN_MLP::load(trainPathCV);
    }
}

ClassifierManager::ClassifierManager()
{


}


