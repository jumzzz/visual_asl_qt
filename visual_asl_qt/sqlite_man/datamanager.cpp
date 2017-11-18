#include "datamanager.h"

DataManager::DataManager()
{
    m_initDB();

    if(!m_checkColorTable())
        m_createColorTable();
}

bool DataManager::m_checkColorTable()
{
    QSqlDatabase myDB = QSqlDatabase::database();
    bool isThereTable = myDB.tables().contains(TABLE_HSV_FULL);

    return isThereTable;
}

void DataManager::m_createColorTable()
{
    QString queryStr = "CREATE TABLE " + TABLE_HSV_FULL + "(hue INT, sat INT, val INT, type INT)";
    QSqlQuery tableQuery;

    qDebug() << "My query is: " << queryStr;

    tableQuery.exec(queryStr);

}

void DataManager::m_initDB()
{
    QStringList dbL = QSqlDatabase::connectionNames();

    for(const QString &str : dbL)
        qDebug() << "Connection: " << str;

    if(dbL.length() <= 0)
    {
        m_mainDB = QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath = QLatin1String("vasl_db.sqlite");
        m_mainDB.setDatabaseName(dbPath);
        m_mainDB.open();
    }
}

void DataManager::updateHSVValues(const cv::Mat &img, const bool &skin)
{

    int i = 0;
    if(img.channels() == 3)
    {
        QSqlQuery query;
        QString queryStr("INSERT INTO " + TABLE_HSV_FULL +" VALUES(?, ?, ?, ?)");

        query.prepare(queryStr);

        int skinVal = skin ? 1: 0;

        cv::Mat img_clone = img.clone();

        cv::MatConstIterator_<cv::Vec3b> it, end;

        std::vector<uchar> hues(img_clone.rows*img_clone.cols);
        std::vector<uchar> sats(img_clone.rows*img_clone.cols);
        std::vector<uchar> vals(img_clone.rows*img_clone.cols);
        std::vector<int> skvals(img_clone.rows*img_clone.cols);

        int i = 0;

        std::vector<cv::Mat> splitted;
        cv::split(img_clone, splitted);

        hues.assign(splitted[0].datastart, splitted[0].dataend);
        sats.assign(splitted[1].datastart, splitted[1].dataend);
        vals.assign(splitted[2].datastart, splitted[2].dataend);



        QVariantList hList;
        QVariantList sList;
        QVariantList vList;
        QVariantList svList;

        for(const uchar &h : hues)
            hList << (int)h;

        for(const uchar &s : sats)
            sList << (int)s;

        for(const uchar &v : vals)
            vList << (int)v;



        //        for(const int &h : hues)
        //            hList << h;

        //        for(const int &s : sats)
        //            sList << s;

        //        for(const int &v : vals)
        //            vList << v;

        for(const int &s : skvals)
            svList << s;

        query.addBindValue(hList);
        query.addBindValue(sList);
        query.addBindValue(vList);
        query.addBindValue(svList);

        qDebug() << "Starting to insert values: ";
        if(!query.execBatch())
            qDebug() << query.lastError();
        else
            qDebug() << "Query Success!";

        qDebug() << "Insert Ends: ";
//        updateHSVValues(values, skin);
    }

}

void DataManager::updateHSVValues(const std::vector<cv::Vec3b> &values, const bool &skin)
{
//    int counter = 0;
    QSqlQuery query;
    QString queryStr("INSERT INTO " + TABLE_HSV_FULL +" VALUES(?, ?, ?, ?)");

    query.prepare(queryStr);

    int skinVal = skin ? 1: 0;


    std::vector<int> hues(values.size());
    std::vector<int> sats(values.size());
    std::vector<int> vals(values.size());
    std::vector<int> skvals(values.size(), skinVal);

    int i = 0;

    for(const cv::Vec3b v3 : values)
    {
        hues[i] = (int)v3[0];
        sats[i] = (int)v3[1];
        vals[i] = (int)v3[2];
        skvals[i++] = skinVal;
    }

    QVariantList hList;
    QVariantList sList;
    QVariantList vList;
    QVariantList svList;

    for(const int &h : hues)
        hList << h;

    for(const int &s : sats)
        sList << s;

    for(const int &v : vals)
        vList << v;

    for(const int &s : skvals)
        svList << s;

    query.addBindValue(hList);
    query.addBindValue(sList);
    query.addBindValue(vList);
    query.addBindValue(svList);


    if(!query.execBatch())
        qDebug() << query.lastError();
    else
        qDebug() << "Query Success!";

}

void DataManager::updateHSVRow(const cv::Vec3b &values, const bool &skin)
{
    QSqlQuery query;

    int type = skin ? 1 : 0;
    QString queryStr("INSERT INTO " + TABLE_HSV_FULL +"(hue, sat, val, type) VALUES(:hue, :sat, :val, :type)");
//    QString queryStr("INSERT INTO " + TABLE_HSV_FULL +" VALUES(?, ?, ?, ?)");


    query.prepare(queryStr);
    query.bindValue(":hue",(int)(values[0]));
    query.bindValue(":sat",(int)(values[1]));
    query.bindValue(":val", (int)(values[2]));
    query.bindValue(":type", (int)(type));
    bool executed = query.exec();

    if (executed) qDebug() << "Query Success!";
    else qDebug() << "Query Fail!";
}
