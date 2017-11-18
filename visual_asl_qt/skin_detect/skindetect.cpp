#include "skindetect.h"

SkinDetect::SkinDetect()
{

}

//SkinDetect::SkinDetect(FileManager *fm, int nbins)
//{

//}

void SkinDetect::activate(FileManager *fm, int nbins)
{
    m_initBringdownTable();
    m_init3DLUT();

    m_nbins = nbins;

    std::map<QString, vector<cv::Vec3i>> skin_map = fm->extractRawSkinData();
    std::map<QString, vector<cv::Vec3i>> nskin_map = fm->extractRawNSkinData();

    /// reduce each of them to number of bins
    ///

//    vector<cv::Vec3i> skin_bgr_hist(skin_map.at(KEY_BGR).size());
//    vector<cv::Vec3i> skin_rgb_hist(skin_map.at(KEY_RGB).size());
//    vector<cv::Vec3i> skin_hsv_hist(skin_map.at(KEY_HSV).size());
    vector<cv::Vec3i> skin_hsvFull_hist(skin_map.at(KEY_HSV_FULL).size());
//    vector<cv::Vec3i> skin_lab_hist(skin_map.at(KEY_LAB).size());
//    vector<cv::Vec3i> skin_luv_hist(skin_map.at(KEY_LUV).size());

//    vector<cv::Vec3i> nskin_bgr_hist(nskin_map.at(KEY_BGR).size());
//    vector<cv::Vec3i> nskin_rgb_hist(nskin_map.at(KEY_RGB).size());
//    vector<cv::Vec3i> nskin_hsv_hist(nskin_map.at(KEY_HSV).size());
    vector<cv::Vec3i> nskin_hsvFull_hist(nskin_map.at(KEY_HSV_FULL).size());
//    vector<cv::Vec3i> nskin_lab_hist(nskin_map.at(KEY_LAB).size());
//    vector<cv::Vec3i> nskin_luv_hist(nskin_map.at(KEY_LUV).size());


//    scaleToHist(skin_map.at(KEY_BGR), skin_bgr_hist);
//    scaleToHist(skin_map.at(KEY_RGB), skin_rgb_hist);
//    scaleToHist(skin_map.at(KEY_HSV), skin_hsv_hist);
    scaleToHist(skin_map.at(KEY_HSV_FULL), skin_hsvFull_hist);
//    scaleToHist(skin_map.at(KEY_LAB), skin_lab_hist);
//    scaleToHist(skin_map.at(KEY_LUV), skin_luv_hist);


//    scaleToHist(nskin_map.at(KEY_BGR), nskin_bgr_hist);
//    scaleToHist(nskin_map.at(KEY_RGB), nskin_rgb_hist);
//    scaleToHist(nskin_map.at(KEY_HSV), nskin_hsv_hist);
    scaleToHist(nskin_map.at(KEY_HSV_FULL), nskin_hsvFull_hist);
//    scaleToHist(nskin_map.at(KEY_LAB), nskin_lab_hist);
//    scaleToHist(nskin_map.at(KEY_LUV), nskin_luv_hist);


//    updateLUT(skin_bgr_hist, skin_bgr);
//    updateLUT(skin_rgb_hist, skin_rgb);
//    updateLUT(skin_hsv_hist, skin_hsv);
    updateLUT(skin_hsvFull_hist, skin_hsvFull);
//    updateLUT(skin_bgr_hist, skin_lab);
//    updateLUT(skin_bgr_hist, skin_luv);

//    updateLUT(nskin_bgr_hist, nskin_bgr);
//    updateLUT(nskin_rgb_hist, nskin_rgb);
//    updateLUT(nskin_hsv_hist, nskin_hsv);
    updateLUT(nskin_hsvFull_hist, nskin_hsvFull);
//    updateLUT(nskin_bgr_hist, nskin_lab);
//    updateLUT(nskin_bgr_hist, nskin_luv);


//    ts_bgr = 0.0f;
//    ts_rgb = 0.0f;
//    ts_hsv = 0.0f;
    ts_hsvFull = 0.0f;
//    ts_lab = 0.0f;
//    ts_luv = 0.0f;

//    tn_bgr = 0.0f;
//    tn_rgb = 0.0f;
//    tn_hsv = 0.0f;
    tn_hsvFull = 0.0f;
//    tn_lab = 0.0f;
//    tn_luv = 0.0f;

    qDebug() << "Calculating Ts and Tn";

    for(int c0 = 0; c0 < MAX_SKIN_INDEX; c0++)
    {
        for (int c1 = 0; c1 < MAX_SKIN_INDEX; c1++)
        {
            for (int c2 = 0; c2 < MAX_SKIN_INDEX; c2++)
            {
//                ts_bgr += (float)skin_bgr[c0][c1][c2];
//                ts_rgb += (float)skin_rgb[c0][c1][c2];
//                ts_hsv += (float)skin_hsv[c0][c1][c2];
                ts_hsvFull += (float)skin_hsvFull[c0][c1][c2];
//                ts_lab += (float)skin_lab[c0][c1][c2];
//                ts_luv += (float)skin_luv[c0][c1][c2];

//                tn_bgr += (float)nskin_bgr[c0][c1][c2];
//                tn_rgb += (float)nskin_rgb[c0][c1][c2];
//                tn_hsv += (float)nskin_hsv[c0][c1][c2];
                tn_hsvFull += (float)nskin_hsvFull[c0][c1][c2];
//                tn_lab += (float)nskin_lab[c0][c1][c2];
//                tn_luv += (float)nskin_luv[c0][c1][c2];

            }
        }
    }

//    float den_bgr = ts_bgr + tn_bgr;
//    float den_rgb = ts_rgb + tn_rgb;
//    float den_hsv = ts_hsv + tn_hsv;
    float den_hsvFull = ts_hsvFull + tn_hsvFull;
//    float den_lab = ts_lab + tn_bgr;
//    float den_luv = ts_luv + tn_bgr;

    float abs_min = 0.5;

//    ps_bgr = den_bgr >= abs_min ? ts_bgr/den_bgr : 0.0f;
//    ps_rgb = den_rgb >= abs_min ? ts_rgb/den_rgb : 0.0f;
//    ps_hsv = den_hsv >= abs_min ? ts_hsv/den_hsv : 0.0f;
    ps_hsvFull = den_hsvFull >= abs_min ? ts_hsvFull/den_hsvFull : 0.0f;
//    ps_lab = den_lab >= abs_min ? ts_lab/den_lab : 0.0f;
//    ps_luv = den_luv >= abs_min ? ts_luv/den_luv : 0.0f;

//    pn_bgr = 1.0f - ps_bgr;
//    pn_rgb = 1.0f - ps_rgb;
//    pn_hsv = 1.0f - ps_hsv;
    pn_hsvFull = 1.0f - ps_hsvFull;
//    pn_lab = 1.0f - ps_lab;
//    pn_luv = 1.0f - ps_luv;

    qDebug() << "Loading Done!";
}


void SkinDetect::scaleToHist(vector<cv::Vec3i> &input, vector<cv::Vec3i> &output)
{
//    if(input.size() == output.size())
//    {
        for(int i = 0; i < input.size(); i++)
        {
            cv::Vec3i tmp = input[i];

            ushort c0 = getBringDown(tmp[0]);
            ushort c1 = getBringDown(tmp[1]);
            ushort c2 = getBringDown(tmp[2]);

            cv::Vec3i vec(c0,c1,c2);

            output.push_back(vec);
        }

        qDebug() << "Histogram Size: " <<  output.size();

//    }
}

void SkinDetect::filterSkin(cv::Mat &input, cv::Mat &out, int thresh)
{

//    in_mat = input.getHSVFull().getMat(cv::ACCESS_FAST).clone();
    out = cv::Mat(input.rows,input.cols, input.depth());
    filterFromLUT(input, out, skin_hsvFull, thresh);
}

void SkinDetect::filterSkin(MatConverter &input, cv::Mat &out, SkinSpace ss)
{
    cv::Mat in_mat;
    switch(ss)
    {

    case BGR :
          {
              in_mat = input.getBGR().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_bgr,nskin_bgr, ts_bgr,tn_bgr,ps_bgr,pn_bgr);

          }

        break;
    case RGB :
          {
              in_mat = input.getRGB().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_rgb,nskin_rgb, ts_rgb,tn_rgb,ps_rgb,pn_rgb);

          }

        break;
    case HSV :
          {

              in_mat = input.getHSV().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_hsv,nskin_hsv, ts_hsv,tn_hsv,ps_hsv,pn_hsv);
          }
        break;
    case HSV_FULL :
          {

              in_mat = input.getHSVFull().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_hsvFull,nskin_hsvFull, ts_hsvFull,tn_hsvFull,ps_hsvFull,pn_hsvFull);

          }
        break;
    case LAB :
          {

              in_mat = input.getLab().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_lab,nskin_lab, ts_lab,tn_lab,ps_lab,pn_lab);

          }
        break;
    case LUV :
          {

              in_mat = input.getLUV().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_luv,nskin_luv, ts_luv,tn_luv,ps_luv,pn_luv);
          }
        break;
    default:
        break;
    }
}

void SkinDetect::filterSkin(MatConverter &input, cv::Mat &out, int thresh, SkinSpace ss)
{
    cv::Mat in_mat;
    switch(ss)
    {

    case BGR :
          {
              in_mat = input.getBGR().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_bgr,thresh);

          }

        break;
    case RGB :
          {
              in_mat = input.getRGB().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_rgb, thresh);

          }

        break;
    case HSV :
          {

              in_mat = input.getHSV().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_hsv, thresh);

          }
        break;
    case HSV_FULL :
          {

              in_mat = input.getHSVFull().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_hsvFull, thresh);

          }
        break;
    case LAB :
          {

              in_mat = input.getLab().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_lab, thresh);

          }
        break;
    case LUV :
          {

              in_mat = input.getLUV().getMat(cv::ACCESS_FAST).clone();
              out = cv::Mat(in_mat.rows,in_mat.cols, in_mat.depth());
              filterFromLUT(in_mat, out, skin_luv, thresh);
          }
        break;
    default:
        break;
    }
}

void SkinDetect::filterFromLUT(cv::Mat &input, cv::Mat &output, vector<vector<vector<ulong> > > &stable, vector<vector<vector<ulong> > > &ntable, float ts, float tn, float ps, float pn)
{
    for(ushort y=0;y<input.rows;y++)
    {
        for(ushort x=0;x<input.cols;x++)
        {
            // get pixel
            cv::Vec3b color = input.at<cv::Vec3b>(cv::Point(x,y));


            int c0 = getBringDown((ushort)color[0]);
            int c1 = getBringDown((ushort)color[1]);
            int c2 = getBringDown((ushort)color[2]);

            float ns = stable[c0][c1][c2];
            float nn = ntable[c0][c1][c2];

            float pcs = ts > 0.5f ? ns/ts : 0.0f;
            float pcn = tn > 0.5f ? nn/tn : 0.0f;


            float pSkin = (pcs*ps)/(pcs*ps + pcn*pn);
//            float pNSkin = (pcn*pn)/(pcs*ps + pcn*pn);
            float pNSkin = 1 - pSkin;


            if (pSkin > pNSkin)
                output.at<uchar>(cv::Point(x,y)) = 255;

        }
    }

    cv::medianBlur(output,output,5);



}

void SkinDetect::filterFromLUT(cv::Mat &input, cv::Mat &output, vector<vector<vector<ulong>>> &table, int thresh)
{
    for(ushort y=0;y<input.rows;y++)
    {
        for(ushort x=0;x<input.cols;x++)
        {
            // get pixel
            cv::Vec3b color = input.at<cv::Vec3b>(cv::Point(x,y));


            int c0 = getBringDown((ushort)color[0]);
            int c1 = getBringDown((ushort)color[1]);
            int c2 = getBringDown((ushort)color[2]);


            if (table[c0][c1][c2] > thresh)
                output.at<uchar>(cv::Point(x,y)) = 255;
            else
                output.at<uchar>(cv::Point(x,y)) = 0;


            // ... do something to the color ....

            // set pixel
//            image.at<Vec3b>(Point(x,y)) = color;
        }
    }

    int erosion_size = 1;
    cv::Size ksizeErode(2*erosion_size + 1, 2*erosion_size + 1);
    cv::Point ptErode(erosion_size, erosion_size);
    cv::Mat eErode = cv::getStructuringElement(cv::MORPH_CROSS, ksizeErode, ptErode );

    cv::erode(output,output,eErode);

    int dilation_size = 2;
    cv::Size ksizeDilate(2*dilation_size + 1, 2*dilation_size + 1);
    cv::Point ptDilate(dilation_size, dilation_size);
    cv::Mat eDilate = cv::getStructuringElement(cv::MORPH_ELLIPSE, ksizeDilate, ptDilate);

    cv::dilate(output,output,eDilate,cv::Point(-1,-1),2);


    cv::medianBlur(output,output,5);
}

void SkinDetect::updateLUT(const vector<cv::Vec3i> &input, vector<vector<vector<ulong>>> &table)
{
    for(const cv::Vec3i &vec : input)
    {
        ushort c0 = vec[0];
        ushort c1 = vec[1];
        ushort c2 = vec[2];

        bool val0 = c0 >= 0 && c0 < MAX_SKIN_INDEX;
        bool val1 = c1 >= 0 && c1 < MAX_SKIN_INDEX;
        bool val2 = c2 >= 0 && c2 < MAX_SKIN_INDEX;

        if (val0 && val1 && val2)
            table[c0][c1][c2] += 1;

    }
}




void SkinDetect::m_init3DTable(vector<vector<vector<ulong>>> &table)
{
    table.resize(MAX_SKIN_INDEX);

    for(ushort i = 0; i < MAX_SKIN_INDEX; i++)
    {
        table[i].resize(MAX_SKIN_INDEX);

        for(ushort j = 0; j < MAX_SKIN_INDEX; j++)
        {
            table[i][j].resize(MAX_SKIN_INDEX);
        }
    }

    for(ushort i = 0; i < MAX_SKIN_INDEX; i++ )
        for(ushort j = 0; j < MAX_SKIN_INDEX; j++ )
            for(ushort k = 0; k < MAX_SKIN_INDEX; k++ )
                table[i][j][k] = 0;


}

void SkinDetect::m_init3DLUT()
{
//    m_init3DTable(skin_bgr);
//    m_init3DTable(skin_rgb);
//    m_init3DTable(skin_hsv);
    m_init3DTable(skin_hsvFull);
//    m_init3DTable(skin_lab);
//    m_init3DTable(skin_luv);

//    m_init3DTable(nskin_bgr);
//    m_init3DTable(nskin_rgb);
//    m_init3DTable(nskin_hsv);
    m_init3DTable(nskin_hsvFull);
//    m_init3DTable(nskin_lab);
//    m_init3DTable(nskin_luv);
}

int SkinDetect::m_bringdown(ushort val)
{
    if(val <= 0) return 0;
    else
    {
        ushort offset = val % m_nbins;
        ushort result  = val - offset;

        if (result <= 0) return 0;
        else
            return result;
    }
}

int SkinDetect::getBringDown(ushort val)
{
   if(val >= 0 && val < MAX_BRINGDOWN_INDEX)
    return bringdown_table[val];
   else
    return 0;
}

void SkinDetect::m_initBringdownTable()
{
    bringdown_table.resize(MAX_BRINGDOWN_INDEX);

    for(ushort i = 0; i < MAX_BRINGDOWN_INDEX; i++)
    {
        bringdown_table[i] = m_bringdown(i);
    }

    qDebug() << "Test Debugger";
}



