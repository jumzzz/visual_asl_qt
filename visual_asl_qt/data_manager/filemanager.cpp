#include "filemanager.h"

FileManager::FileManager()
{
   dirPath_app = QCoreApplication::applicationDirPath();

   m_mkSkinDir();
   m_mkSkinFiles();

   m_mkHandDetectDir();

}


QString FileManager::get_dirPathSegmentHand(){
    return dirPathSegmentHand;
}
QString FileManager::get_dirPathSegmentHandGS(){
    return dirPathSegmentHandGS;
}
QString FileManager::get_dirPathSegmentHandLaplacian(){
    return dirPathSegmentHandLaplacian;
}
QString FileManager::get_dirPathSegmentHandSobel(){
    return dirPathSegmentHandSobel;
}
QString FileManager::get_dirPathSegmentHandCanny(){
    return dirPathSegmentHandCanny;
}
QString FileManager::get_dirPathSegmentBG(){
    return dirPathSegmentBG;
}
QString FileManager::get_dirPathSegmentBGGS(){
    return dirPathSegmentBGGS;
}
QString FileManager::get_dirPathSegmentBGLaplacian(){
    return dirPathSegmentBGLaplacian;
}
QString FileManager::get_dirPathSegmentBGSobel(){
    return dirPathSegmentBGSobel;
}
QString FileManager::get_dirPathSegmentBGCanny(){
    return dirPathSegmentBGCanny;
}


std::map<QString, std::vector<cv::Vec3i>> FileManager::extractRawNSkinData()
{
    std::map<QString, std::vector<cv::Vec3i>> result;

    QStringList ls_bgr = extractRawStrList(filePath_skin_bgr);
    QStringList ls_rgb = extractRawStrList(filePath_skin_rgb);
    QStringList ls_hsv = extractRawStrList(filePath_skin_hsv);
    QStringList ls_hsvFull = extractRawStrList(filePath_skin_hsvFull);
    QStringList ls_lab = extractRawStrList(filePath_skin_lab);
    QStringList ls_luv = extractRawStrList(filePath_skin_luv);

    std::vector<cv::Vec3i> lr_bgr = extractDataFromList(ls_bgr,false);
    std::vector<cv::Vec3i> lr_rgb = extractDataFromList(ls_rgb,false);
    std::vector<cv::Vec3i> lr_hsv = extractDataFromList(ls_hsv,false);
    std::vector<cv::Vec3i> lr_hsvFull = extractDataFromList(ls_hsvFull,false);
    std::vector<cv::Vec3i> lr_lab = extractDataFromList(ls_lab,false);
    std::vector<cv::Vec3i> lr_luv = extractDataFromList(ls_luv,false);

    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_BGR, lr_bgr));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_RGB, lr_rgb));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_HSV, lr_hsv));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_HSV_FULL, lr_hsvFull));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_LAB, lr_lab));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_LUV, lr_luv));

    return result;
}



std::map<QString, std::vector<cv::Vec3i>> FileManager::extractRawSkinData()
{
    std::map<QString, std::vector<cv::Vec3i>> result;

//    QStringList ls_bgr = extractRawStrList(filePath_skin_bgr);
//    QStringList ls_rgb = extractRawStrList(filePath_skin_rgb);
//    QStringList ls_hsv = extractRawStrList(filePath_skin_hsv);
    QStringList ls_hsvFull = extractRawStrList(filePath_skin_hsvFull);
//    QStringList ls_lab = extractRawStrList(filePath_skin_lab);
//    QStringList ls_luv = extractRawStrList(filePath_skin_luv);

//    std::vector<cv::Vec3i> lr_bgr = extractDataFromList(ls_bgr,true);
//    std::vector<cv::Vec3i> lr_rgb = extractDataFromList(ls_rgb,true);
//    std::vector<cv::Vec3i> lr_hsv = extractDataFromList(ls_hsv,true);
    std::vector<cv::Vec3i> lr_hsvFull = extractDataFromList(ls_hsvFull,true);
//    std::vector<cv::Vec3i> lr_lab = extractDataFromList(ls_lab,true);
//    std::vector<cv::Vec3i> lr_luv = extractDataFromList(ls_luv,true);

//    qDebug() << "BGR Vector Size: " << lr_bgr.size();
//    qDebug() << "RGB Vector Size: " << lr_rgb.size();
//    qDebug() << "HSV Vector Size: " << lr_hsv.size();
    qDebug() << "HSV FULL Vector Size: " << lr_hsvFull.size();
//    qDebug() << "LAB Vector Size: " << lr_lab.size();
//    qDebug() << "LUV Vector Size: " << lr_luv.size();
////    qDebug() << "BGR Vector Size: " << lr_bgr.size();

//    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_BGR, lr_bgr));
//    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_RGB, lr_rgb));
//    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_HSV, lr_hsv));
    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_HSV_FULL, lr_hsvFull));
//    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_LAB, lr_lab));
//    result.insert(std::pair<QString, std::vector<cv::Vec3i>>(KEY_LUV, lr_luv));


    return result;
}

QStringList FileManager::extractRawStrList(const QString &path)
{
    QFile inputFile(path);

    QStringList resultList;

    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        while(!in.atEnd())
        {
            QString lineData = in.readLine();
            resultList.append(lineData);
        }


        inputFile.close();
    }

    return resultList;
}

std::vector<cv::Vec3i> FileManager::extractDataFromList(QStringList &rawList, bool isSkin)
{

    qDebug() << "Input Raw List Size: " << rawList.length();

    int skinIndicator = isSkin ? 1 : 0;

    std::vector<cv::Vec3i> result;

    for(QString &line : rawList)
    {
        QStringList split = line.split(",");

        if(split.length() == 4)
        {
            QString lineSkinStr = split.at(3);
            int lineSkin = lineSkinStr.toInt();

            if (lineSkin == skinIndicator)
            {
                QString str_c0 = split.at(0);
                QString str_c1 = split.at(1);
                QString str_c2 = split.at(2);

                int c0 = str_c0.toInt();
                int c1 = str_c1.toInt();
                int c2 = str_c2.toInt();

                bool val0 = c0 <= 256 && c0 >= 0;
                bool val1 = c1 <= 256 && c1 >= 0;
                bool val2 = c2 <= 256 && c2 >= 0;

                if(val0 && val1 && val2)
                {
                    cv::Vec3i vec(c0,c1,c2);
                    result.push_back(vec);
                }
            }

        }
    }

    return result;
}

void FileManager::m_mkHandDetectDir()
{
    QDir segmentDir(dirPath_app + "/"+SEGMENT);

    if(!segmentDir.exists())
        segmentDir.mkdir(dirPath_app + "/"+SEGMENT);

    QDir segmentHand(segmentDir.path() + "/" + SEGMENT_HAND);
    QDir segmentBG(segmentDir.path() + "/" + SEGMENT_BG);

    if (!segmentHand.exists())
        segmentHand.mkdir(segmentHand.path());

    if (!segmentBG.exists())
        segmentBG.mkdir(segmentBG.path());


    dirPathSegmentHandGS = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_HAND + SEGMENT_GS;
    dirPathSegmentHandLaplacian = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_HAND + SEGMENT_LAPLACIAN;
    dirPathSegmentHandSobel = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_HAND + SEGMENT_SOBEL;
    dirPathSegmentHandCanny = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_HAND + SEGMENT_CANNY;

    dirPathSegmentBGGS = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_BG + SEGMENT_GS;
    dirPathSegmentBGLaplacian = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_BG + SEGMENT_LAPLACIAN;
    dirPathSegmentBGSobel = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_BG + SEGMENT_SOBEL;
    dirPathSegmentBGCanny = dirPath_app + "/" + SEGMENT + "/" + SEGMENT_BG + SEGMENT_CANNY;


    dirSegmentHandGS = QDir(dirPathSegmentHandGS);
    dirSegmentHandLaplacian = QDir(dirPathSegmentHandLaplacian);
    dirSegmentHandSobel = QDir(dirPathSegmentHandSobel);
    dirSegmentHandCanny = QDir(dirPathSegmentHandCanny);

    if(!dirSegmentHandGS.exists())
        dirSegmentHandGS.mkdir(dirPathSegmentHandGS);

    if(!dirSegmentHandLaplacian.exists())
        dirSegmentHandLaplacian.mkdir(dirPathSegmentHandLaplacian);

    if(!dirSegmentHandSobel.exists())
        dirSegmentHandSobel.mkdir(dirPathSegmentHandSobel);

    if(!dirSegmentHandCanny.exists())
        dirSegmentHandCanny.mkdir(dirPathSegmentHandCanny);

    dirSegmentBGGS = QDir(dirPathSegmentBGGS);
    dirSegmentBGLaplacian = QDir(dirPathSegmentBGLaplacian);
    dirSegmentBGSobel = QDir(dirPathSegmentBGSobel);
    dirSegmentBGCanny = QDir(dirPathSegmentBGCanny);

    if(!dirSegmentBGGS.exists())
        dirSegmentBGGS.mkdir(dirPathSegmentBGGS);

    if(!dirSegmentBGLaplacian.exists())
        dirSegmentBGLaplacian.mkdir(dirPathSegmentBGLaplacian);

    if(!dirSegmentBGSobel.exists())
        dirSegmentBGSobel.mkdir(dirPathSegmentBGSobel);

    if(!dirSegmentBGCanny.exists())
        dirSegmentBGCanny.mkdir(dirPathSegmentBGCanny);

//    dirPathSegmentBGGS = SEGMENT + "/" + SEGMENT_BG + SEGMENT_GS;



}


void FileManager::m_mkSkinDir()
{
    dirPath_skin_bgr = dirPath_app + "/" + KEY_BGR;
    dirPath_skin_rgb = dirPath_app + "/" + KEY_RGB;
    dirPath_skin_hsv = dirPath_app + "/" + KEY_HSV;
    dirPath_skin_hsvFull = dirPath_app + "/" + KEY_HSV_FULL;
    dirPath_skin_lab = dirPath_app + "/" + KEY_LAB;
    dirPath_skin_luv = dirPath_app + "/" + KEY_LUV;

    dirBGR = QDir(dirPath_skin_bgr);
    dirRGB = QDir(dirPath_skin_rgb);
    dirHSV = QDir(dirPath_skin_hsv);
    dirHSV_FULL = QDir(dirPath_skin_hsvFull);
    dirLAB = QDir(dirPath_skin_lab);
    dirLUV = QDir(dirPath_skin_luv);

    if(!dirBGR.exists())
        dirBGR.mkdir(dirPath_skin_bgr);

    if(!dirRGB.exists())
        dirRGB.mkdir(dirPath_skin_rgb);

    if(!dirHSV.exists())
        dirHSV.mkdir(dirPath_skin_hsv);

    if(!dirHSV_FULL.exists())
        dirHSV_FULL.mkdir(dirPath_skin_hsvFull);

    if(!dirLAB.exists())
        dirLAB.mkdir(dirPath_skin_lab);

    if(!dirLUV.exists())
        dirLUV.mkdir(dirPath_skin_luv);
}

FileManager::~FileManager()
{
//    delete fileBGR;
//    delete fileRGB;
//    delete fileHSV;
//    delete fileHSV_FULL;
//    delete fileLAB;
//    delete fileLUV;
}

QStringList FileManager::matToStringList(const std::vector<cv::Mat> &matList, bool skin)
{
    QStringList listResult;

    int skinVal = skin ? 1 : 0;


    for(const cv::Mat &m : matList)
    {
        const int channels = m.channels();

        if (channels == 3)
        {
            cv::MatConstIterator_<cv::Vec3b> it,end;

            for(it = m.begin<cv::Vec3b>(), end = m.end<cv::Vec3b>(); it != end; ++it)
            {
                int c0 = (uint)((*it)[0]);
                int c1 = (uint)((*it)[1]);
                int c2 = (uint)((*it)[2]);

                QString lineData = QString("%1,%2,%3,%4").arg(c0).arg(c1).arg(c2).arg(skinVal);
                listResult.append(lineData);
            }
        }

    }

    return listResult;

}

void FileManager::appendListToFile(const QStringList &list, QFile &file)
{
      file.open(QIODevice::WriteOnly | QIODevice::Append);

      if(!file.isOpen())
      {
          qDebug() << "Error! Unable to open file! " << file.fileName();
      }
      else
      {
          QTextStream out(&file);

          for(const QString &lineData : list)
          {
              out << lineData;
              out << "\n";
          }

         file.close();
      }
}

void FileManager::saveSkinRawData(const std::map<QString, std::vector<cv::Mat> > &skinData, bool skin)
{
    QStringList listBGR = matToStringList(skinData.at(KEY_BGR), skin);
    QStringList listRGB = matToStringList(skinData.at(KEY_RGB), skin);
    QStringList listHSV = matToStringList(skinData.at(KEY_HSV), skin);
    QStringList listHSVFull = matToStringList(skinData.at(KEY_HSV_FULL), skin);
    QStringList listLAB = matToStringList(skinData.at(KEY_LAB), skin);
    QStringList listLUV = matToStringList(skinData.at(KEY_LUV), skin);

    QFile fileBGR(filePath_skin_bgr);
    QFile fileRGB(filePath_skin_rgb);
    QFile fileHSV(filePath_skin_hsv);
    QFile fileHSV_FULL(filePath_skin_hsvFull);
    QFile fileLAB(filePath_skin_lab);
    QFile fileLUV(filePath_skin_luv);

    appendListToFile(listBGR,fileBGR);
    appendListToFile(listRGB,fileRGB);
    appendListToFile(listHSV,fileHSV);
    appendListToFile(listHSVFull,fileHSV_FULL);
    appendListToFile(listLAB,fileLAB);
    appendListToFile(listLUV,fileLUV);

}

void FileManager::deleteAllSkinFiles()
{
    QFile fileBGR(filePath_skin_bgr);
    QFile fileRGB(filePath_skin_rgb);
    QFile fileHSV(filePath_skin_hsv);
    QFile fileHSV_FULL(filePath_skin_hsvFull);
    QFile fileLAB(filePath_skin_lab);
    QFile fileLUV(filePath_skin_luv);

    fileBGR.remove();
    fileRGB.remove();
    fileHSV.remove();
    fileHSV_FULL.remove();
    fileLAB.remove();
    fileLUV.remove();

}

void FileManager::m_mkSkinFiles()
{
    filePath_skin_bgr = dirPath_skin_bgr + "/bgr.csv";
    filePath_skin_rgb = dirPath_skin_rgb + "/rgb.csv";
    filePath_skin_hsv = dirPath_skin_hsv + "/hsv.csv";
    filePath_skin_hsvFull = dirPath_skin_hsvFull + "/hsvFull.csv";
    filePath_skin_lab = dirPath_skin_lab + "/lab.csv";
    filePath_skin_luv = dirPath_skin_luv + "/luv.csv";

//    QFile fileBGR(filePath_skin_bgr);
//    QFile fileRGB(filePath_skin_rgb);
//    QFile fileHSV(filePath_skin_hsv);
//    QFile fileHSV_FULL(filePath_skin_hsvFull);
//    QFile fileLAB(filePath_skin_lab);
//    QFile fileLUV(filePath_skin_luv);

//    fileBGR.open(QIODevice::WriteOnly);
//    fileRGB.open(QIODevice::WriteOnly);
//    fileHSV.open(QIODevice::WriteOnly);
//    fileHSV_FULL.open(QIODevice::WriteOnly);
//    fileLAB.open(QIODevice::WriteOnly);
//    fileLUV.open(QIODevice::WriteOnly);

//    fileBGR.close();
//    fileRGB.close();
//    fileHSV.close();
//    fileHSV_FULL.close();
//    fileLAB.close();
//    fileLUV.close();



}
