#include "featureextract.h"

//using namespace Features;

void Features::handToArmSegFeatures(const Mat &inputImg, const Rect &roi, Mat &descriptor)
{

    Ptr<FastFeatureDetector> ffd = FastFeatureDetector::create(10);
    Ptr<FREAK> freak = FREAK::create();

    Mat segmented(inputImg, roi);

    vector<KeyPoint> kp;

    ffd->detect(segmented,kp);
    freak->compute(segmented,kp, descriptor);

}


void Features::lineDescriptorFeatures(const Mat &inputImg, vector<KeyLine> &keyLines)
{
    cv::Mat mask = Mat::ones( inputImg.size(), CV_8UC1 );

    Ptr<BinaryDescriptor> bd = BinaryDescriptor::createBinaryDescriptor();
    bd->detect( inputImg, keyLines, mask );

}

void Features::lineDescriptorFeatures(const Mat &inputImg, const Rect &rect, vector<pair<Point, Point>> &lines)
{

    if (!lines.empty())
        lines.clear();

    Mat croppedImg = cv::Mat(inputImg, rect);
    cv::Mat mask = Mat::ones( croppedImg.size(), CV_8UC1 );

    Ptr<BinaryDescriptor> bd = BinaryDescriptor::createBinaryDescriptor();

    vector<KeyLine> subKeyLines;
    bd->detect(croppedImg, subKeyLines, mask);


    for (int i = 0; i < subKeyLines.size(); i++)
    {
        KeyLine kl = subKeyLines[i];

        if( kl.octave == 0)
        {
            cv::Point pt0 = cv::Point( rect.tl().x + kl.startPointX, rect.tl().y + kl.startPointY );
            cv::Point pt1 = cv::Point( rect.tl().x + kl.endPointX, rect.tl().y + kl.endPointY);

            lines.push_back(pair<Point,Point>(pt0,pt1));
        }
    }
}

void Features::ORBDescriptor(const Mat &inputImg, vector<KeyPoint> &keyPoints)
{
    Ptr<ORB> orb = ORB::create();

    orb->detect(inputImg, keyPoints);
}

vector<float> Features::sideMagnitudeFeatures(const Rect &rect, const vector<KeyLine> &keyLines, int rows, int cols)
{
    std::vector<float> finalFeatures;
    std::vector<cv::Rect> rects = CVMisc::generateSubRects(rect,rows,cols);

    for(const cv::Rect &rect : rects)
    {
        std::vector<float> rectMagnitudes = CVMisc::sideMagnitude(rect, keyLines, 0.05);
        finalFeatures.insert(finalFeatures.end(), rectMagnitudes.begin(), rectMagnitudes.end());
    }

    return finalFeatures;
}

vector<float> Features::sideMagnitudeFeatures(const Rect &rect, const vector<pair<Point,Point>> &lines, int rows, int cols)
{
    vector<float> finalFeatures;
    vector<Rect> rects = CVMisc::generateSubRects(rect,rows,cols);

    for(const Rect &rectIn : rects)
    {
        vector<float> rectMagnitudes = CVMisc::sideMagnitude(rectIn, lines, 0.05);
        finalFeatures.insert(finalFeatures.end(), rectMagnitudes.begin(), rectMagnitudes.end());
    }

    return finalFeatures;
}

vector<float> Features::keypointDensities(const Rect &rect, const vector<KeyPoint> &kps, int rows, int cols)
{
    vector<float> features;

    vector<Rect> rects = CVMisc::generateSubRects(rect, rows, cols);

    for (const Rect &rectIn : rects)
    {
        float orbDensity = CVMisc::orbDensity(rectIn, kps);

        features.push_back(orbDensity);
    }

    return features;


}

vector<float> Features::keypointDensities(const RotatedRect &rect, const vector<KeyPoint> &kps)
{
    float numOfKPs = 0.0f;
    float totalKPAreas = 0.0f;
    float aveAngle = 0.0f;

    for (const KeyPoint &kp : kps)
    {
//        float orbDensity = CVMisc:rectIn, kps);
        cv::Point pt(kp.pt);

        if (CVMisc::pointInRect(rect, pt))
        {
            numOfKPs += 1.0f;
            totalKPAreas += kp.size*kp.size;
            aveAngle += kp.angle;
        }
//        features.push_back(orbDensity);
    }

    if (numOfKPs != 0.0f)
    aveAngle = aveAngle/numOfKPs;
    else
        aveAngle = 0.0f;

    return {numOfKPs, totalKPAreas, aveAngle};
}

vector<float> Features::keypointDensities(const RotatedRect &rect, const vector<KeyPoint> &kps, vector<KeyPoint> &outKPs)
{

    float numOfKPs = 0.0f;
    float totalKPAreas = 0.0f;
    float aveAngle = 0.0f;

    if (!outKPs.empty())
        outKPs.clear();

    for (const KeyPoint &kp : kps)
    {
//        float orbDensity = CVMisc:rectIn, kps);
        cv::Point pt(kp.pt);

        if (CVMisc::pointInRect(rect, pt))
        {
            numOfKPs += 1.0f;
            totalKPAreas += kp.size*kp.size;
            aveAngle += kp.angle;

            outKPs.push_back(kp);
        }
//        features.push_back(orbDensity);
    }

    if (numOfKPs != 0.0f)
    aveAngle = aveAngle/numOfKPs;
    else
        aveAngle = 0.0f;

    return {numOfKPs, totalKPAreas, aveAngle};
}




vector<float> Features::ellipticFeaturesTotal(const Rect &rect, const vector<pair<Point, Point> > &lines, int rows, int cols, vector<RotatedRect> &ellipses)
{
    vector<float> finalFeatures;
    vector<Rect> rects = CVMisc::generateSubRects(rect, rows, cols);

    if (!ellipses.empty())
        ellipses.clear();

    for(const cv::Rect &rect : rects)
    {
        RotatedRect ellipse;
        vector<float> rectMagnitudes = CVMisc::elipticalFeature(rect, lines, ellipse);
        finalFeatures.insert(finalFeatures.end(), rectMagnitudes.begin(), rectMagnitudes.end());
        ellipses.push_back(ellipse);

    }

    return finalFeatures;

}

vector<float> Features::hogDescriptor(const Mat &input)
{
    cv::Mat resized;

    cv::Size size = cv::Size(64,8);

    cv::resize(input, resized, size);
    cv::Size winSize(64,8);
    cv::Size blockSize(8,8);
    cv::Size blockStride(4,4);
    cv::Size cellSize(4,4);
    int nBins = 9;

    cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize,nBins);

    std::vector<cv::Point> positions;
    std::vector<float> descriptor;
    hog.compute(resized,descriptor,cv::Size(),cv::Size(),positions);

    return descriptor;

}


