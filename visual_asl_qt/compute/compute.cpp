#include "compute.h"
#include <QDebug>
#include <QCoreApplication>

CVMisc::CVMisc()
{

}

int CVMisc::dotProduct(const cv::Point &pt0, const cv::Point &pt1)
{
    return (pt0.x*pt1.x + pt0.y*pt1.y);
}




int CVMisc::perpDotProduct(const cv::Point &pt0, const cv::Point &pt1)
{
    return (pt0.x*pt1.x - pt0.y*pt1.y);
}

bool CVMisc::pointInRect(const std::vector<cv::Point> &rectPts, const cv::Point &pt)
{
    if (rectPts.size() != 4)
    {
        return false;
    }
    else
    {
        cv::Point ptA((int)rectPts[0].x, (int)rectPts[0].y);
        cv::Point ptB((int)rectPts[1].x, (int)rectPts[1].y);
        cv::Point ptC((int)rectPts[2].x, (int)rectPts[2].y);

        cv::Point ptAB(ptB.x - ptA.x, ptB.y - ptA.y);
        cv::Point ptAM(pt.x - ptA.x, pt.y - ptA.y);
        cv::Point ptBC(ptC.x - ptB.x, ptC.y - ptB.y);
        cv::Point ptBM(pt.x - ptB.x, pt.y - ptB.y);

        int dp0 = dotProduct(ptAB, ptAM);
        int dp1 = dotProduct(ptAB, ptAB);
        int dp2 = dotProduct(ptBC, ptBM);
        int dp3 = dotProduct(ptBC, ptBC);

        bool c0 = 0 <= dp0;
        bool c1 = dp0 <= dp1;
        bool c2 = 0 <= dp2;
        bool c3 = dp2 <= dp3;

        return c0 && c1 && c2 && c3;

    }
}

bool CVMisc::pointInRect(const cv::RotatedRect &rect, const cv::Point &pt)
{
    cv::Point2f v[4];
    rect.points(v);

    cv::Point ptA((int)v[0].x, (int)v[0].y);
    cv::Point ptB((int)v[1].x, (int)v[1].y);
    cv::Point ptC((int)v[2].x, (int)v[2].y);

    cv::Point ptAB(ptB.x - ptA.x, ptB.y - ptA.y);
    cv::Point ptAM(pt.x - ptA.x, pt.y - ptA.y);
    cv::Point ptBC(ptC.x - ptB.x, ptC.y - ptB.y);
    cv::Point ptBM(pt.x - ptB.x, pt.y - ptB.y);

    int dp0 = dotProduct(ptAB, ptAM);
    int dp1 = dotProduct(ptAB, ptAB);
    int dp2 = dotProduct(ptBC, ptBM);
    int dp3 = dotProduct(ptBC, ptBC);

    bool c0 = 0 <= dp0;
    bool c1 = dp0 <= dp1;
    bool c2 = 0 <= dp2;
    bool c3 = dp2 <= dp3;

    return c0 && c1 && c2 && c3;
}

bool CVMisc::pointInRect(const cv::Rect2d &rect, const cv::KeyPoint &kp)
{
    int x0 = rect.x;
    int y0 = rect.y;

    int x1 = rect.x + rect.width;
    int y1 = rect.y + rect.height;


    cv::Point ptA(x0, y0);
    cv::Point ptB(x1, y0);
    cv::Point ptC(x0, y1);

    cv::Point ptAB(ptB.x - ptA.x, ptB.y - ptA.y);
    cv::Point ptAM(kp.pt.x - ptA.x, kp.pt.y - ptA.y);
    cv::Point ptBC(ptC.x - ptB.x, ptC.y - ptB.y);
    cv::Point ptBM(kp.pt.x - ptB.x, kp.pt.y - ptB.y);

    int dp0 = dotProduct(ptAB, ptAM);
    int dp1 = dotProduct(ptAB, ptAB);
    int dp2 = dotProduct(ptBC, ptBM);
    int dp3 = dotProduct(ptBC, ptBC);

    bool c0 = 0 <= dp0;
    bool c1 = dp0 <= dp1;
    bool c2 = 0 <= dp2;
    bool c3 = dp2 <= dp3;

    return c0 && c1 && c2 && c3;
}

bool CVMisc::pointInRect(const cv::Rect &rect, const cv::Point &pt)
{
    //  p00(x0,y0)---------p10(x1,y0)
    //    |                   |
    //    |                   |
    //  p01(x0,y1)---------p11(x1,x1)

    int x0 = rect.x;
    int y0 = rect.y;

    int x1 = rect.x + rect.width;
    int y1 = rect.y + rect.height;


    cv::Point ptA(x0, y0);
    cv::Point ptB(x1, y0);
    cv::Point ptC(x0, y1);

    cv::Point ptAB(ptB.x - ptA.x, ptB.y - ptA.y);
    cv::Point ptAM(pt.x - ptA.x, pt.y - ptA.y);
    cv::Point ptBC(ptC.x - ptB.x, ptC.y - ptB.y);
    cv::Point ptBM(pt.x - ptB.x, pt.y - ptB.y);

    int dp0 = dotProduct(ptAB, ptAM);
    int dp1 = dotProduct(ptAB, ptAB);
    int dp2 = dotProduct(ptBC, ptBM);
    int dp3 = dotProduct(ptBC, ptBC);

    bool c0 = 0 <= dp0;
    bool c1 = dp0 <= dp1;
    bool c2 = 0 <= dp2;
    bool c3 = dp2 <= dp3;

    return c0 && c1 && c2 && c3;
}

bool CVMisc::allKPsInRect(const cv::Rect2d &rect, const std::vector<cv::KeyPoint> &kps)
{
    if (kps.empty())
        return false;

    for(const cv::KeyPoint &kp : kps)
        if (!pointInRect(rect, kp))
            return false;

    return true;
}


bool CVMisc::pointInCircle(const float &circleRadius, const cv::Point &circleCentroid, const cv::Point &testPoint)
{
    //    float delXSq = (float)(testPoint.x - circleCentroid.x)*(testPoint.x - circleCentroid.x);
    //    float delYSq = (float)(testPoint.y - circleCentroid.y)*(testPoint.y - circleCentroid.y);

    //    return delXSq + delYSq < (circleRadius*circleRadius);

    int delX = abs(testPoint.x - circleCentroid.x);
    int delY = abs(testPoint.y - circleCentroid.y);


    bool cond0 = true;
    bool cond1 = true;

    if (delX > (int)circleRadius)
        cond0 = false;

    if (delY > (int)circleRadius)
        cond1 = false;


    return cond0 && cond1;


}

bool CVMisc::pointInCircle(Circle &circle, const cv::Point &testpoint)
{
    float radius = circle.radius;
    cv::Point centroid = circle.centroid;

    return pointInCircle(radius,centroid, testpoint);
}

cv::Point CVMisc::pointInBet(const cv::Point &p0, const cv::Point &p1, const float &ratio)
{
    float xDel = p1.x - p0.x;
    float yDel = p1.y - p0.y;

    float x = ratio*xDel + p0.x;
    float y = ratio*yDel + p0.y;

    x = round(x);
    y = round(y);

    return cv::Point((int)x,(int)y);
}

float CVMisc::aveKeyPointAngle(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kps)
{
    float angleTot = 0.0f;
    float n = (float)kps.size();

    if (kps.size() == 0)
        return 0.0f;
    else
    {
        for (const cv::KeyPoint &kp : kps)
            angleTot += kp.angle;

        return angleTot/n;
    }

}

float CVMisc::aveKeyPointResp(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kps)
{
    float respTot = 0.0f;
    float n = (float)kps.size();

    if (kps.size() == 0)
        return 0.0f;
    else
    {
        for (const cv::KeyPoint &kp : kps)
            respTot += kp.angle;

        return respTot/n;
    }
}

float CVMisc::aveKeyPointSize(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kps)
{
    float angleTot = 0.0f;
    float n = (float)kps.size();

    if (kps.size() == 0)
        return 0.0f;
    else
    {
        for (const cv::KeyPoint &kp : kps)
            angleTot += kp.angle;

        return angleTot/n;
    }
}



float CVMisc::orbDensity(const cv::Rect &rect, const std::vector<cv::KeyPoint> &kps)
{
    float orbAreas = 0.0f;

    float pi = (float)M_PI;
    float rectArea = rect.area();

    for(const cv::KeyPoint &kp : kps)
    {
        cv::Point pt(kp.pt);

        if (pointInRect(rect, pt))
        {
            float r = kp.size;
            orbAreas += r*r*pi;
        }
    }

    return (orbAreas)/rectArea;
}



std::vector<cv::Rect> CVMisc::generateSubRects(const cv::Rect &rect, int rows, int cols)
{
    int x0 = rect.x;
    int y0 = rect.y;

    int total_width = rect.width;
    int total_height = rect.height;

    int sub_width = total_width/cols;
    int sub_height = total_height/rows;

    std::vector<cv::Rect> subRects(rows*cols, cv::Rect());

    size_t index = 0;

    for (int yDel = 0; yDel < rows; yDel++)
    {
        for (int xDel = 0; xDel < cols; xDel++)
        {
            int x1 = x0 + xDel*sub_width;
            int y1 = y0 + yDel*sub_height;

            cv::Rect subRect(x1,y1,sub_width,sub_height);

            subRects[index++] = subRect;

            if (index >= subRects.size())
                break;
        }

        if (index >= subRects.size())
            break;
    }

    return subRects;
}

bool CVMisc::intersection(const cv::Point2f &A1, const cv::Point2f &A2, const cv::Point2f &B1, const cv::Point2f &B2, cv::Point2f &result)
{
    //    cv::Point2f x = o2 - o1;
    //    cv::Point2f d1 = p1 - o1;
    //    cv::Point2f d2 = p2 - o2;

    //    float cross = d1.x*d2.y - d1.y*d2.x;
    //    if (abs(cross) < /*EPS*/1e-8)
    //        return false;

    //    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    //    r = o1 + d1 * t1;
    //    return true;

    cv::Point a(A2-A1);
    cv::Point b(B2-B1);

    double f = (double)perpDotProduct(a,b);
    if(!f)      // lines are parallel
        return false;

    cv::Point c(B2-A2);
    double aa = (double)perpDotProduct(a,c);
    double bb = (double)perpDotProduct(b,c);

    if(f < 0)
    {
        if(aa > 0)     return false;
        if(bb > 0)     return false;
        if(aa < f)     return false;
        if(bb < f)     return false;
    }
    else
    {
        if(aa < 0)     return false;
        if(bb < 0)     return false;
        if(aa > f)     return false;
        if(bb > f)     return false;
    }


    double out = 1.0 - (aa / f);
    result = ((B2 - B1) * out) + B1;


    return true;
}


std::vector<float> CVMisc::sideMagnitude(const cv::Rect &rect, const std::vector<cv::line_descriptor::KeyLine> &lines, float minThresh)
{

    std::vector<float> magnitudes(4,0.0f);

    float width = (float)rect.width;
    float height = (float)rect.height;

    // first line
    cv::Point2f p11 = cv::Point2f(rect.tl());
    cv::Point2f p12 = cv::Point2f(rect.br().x, rect.tl().y);

    // second line
    cv::Point2f p21 = cv::Point2f(rect.br().x, rect.tl().y);
    cv::Point2f p22 = cv::Point2f(rect.br());

    // third line
    cv::Point2f p31 = cv::Point2f(rect.br());
    cv::Point2f p32 = cv::Point2f(rect.tl().x,rect.br().y);

    // fourth line
    cv::Point2f p41 = cv::Point2f(rect.tl().x,rect.br().y);
    cv::Point2f p42 = cv::Point2f(rect.tl());

    std::vector<cv::Point2f> l1Pts;
    std::vector<cv::Point2f> l2Pts;
    std::vector<cv::Point2f> l3Pts;
    std::vector<cv::Point2f> l4Pts;


    for ( size_t i = 0; i < lines.size(); i++ )
    {
        KeyLine kl = lines[i];

        if( kl.octave == 0)
        {
            cv::Point2f pt0 = cv::Point2f( kl.startPointX, kl.startPointY );
            cv::Point2f pt1 = cv::Point2f( kl.endPointX, kl.endPointY );

            cv::Point2f pti1;
            cv::Point2f pti2;
            cv::Point2f pti3;
            cv::Point2f pti4;

            if ( intersection(p11, p12, pt0,pt1, pti1)) l1Pts.push_back(pti1);
            if ( intersection(p21, p22, pt0,pt1, pti2)) l2Pts.push_back(pti2);
            if ( intersection(p31, p32, pt0,pt1, pti3)) l3Pts.push_back(pti3);
            if ( intersection(p41, p42, pt0, pt1, pti4)) l4Pts.push_back(pti4);

        }
    }

    if (l1Pts.size() > 0)
    {
        std::sort( l1Pts.begin(), l1Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.x < rhs.x;
        });


        float dist = cv::norm(l1Pts.at(0) - p11);
        float n = 1;


        for (size_t i = 0; i < l1Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l1Pts.at(i + 1) - l1Pts.at(i));

            if (distTmp/width > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[0] = n*dist;
    }

    if (l2Pts.size() > 0)
    {
        std::sort( l2Pts.begin(), l2Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.y < rhs.y;
        });


        float dist = cv::norm(l2Pts.at(0) - p21);
        float n = 1;


        for (size_t i = 0; i < l2Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l2Pts.at(i + 1) - l2Pts.at(i));

            if (distTmp/height > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[1] = n*dist;
    }

    if (l3Pts.size() > 0)
    {
        std::sort( l3Pts.begin(), l3Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.x > rhs.x;
        });

        float dist = cv::norm(l3Pts.at(0) - p31);
        float n = 1;


        for (size_t i = 0; i < l3Pts.size() - 1; i++)
        {

            float distTmp = (float)cv::norm(l3Pts.at(i + 1) - l3Pts.at(i));

            if (distTmp/width > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[2] = n*dist;
    }

    if (l4Pts.size() > 0)
    {
        std::sort( l4Pts.begin(), l4Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.y > rhs.y;
        });



        float dist = cv::norm(l4Pts.at(0) - p41);
        float n = 1;


        for (size_t i = 0; i < l4Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l4Pts.at(i + 1) - l4Pts.at(i));

            if (distTmp/height > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[3] = n*dist;
    }

    magnitudes[0] = magnitudes[0]/width;
    magnitudes[1] = magnitudes[1]/height;
    magnitudes[2] = magnitudes[2]/width;
    magnitudes[3] = magnitudes[3]/height;

    return magnitudes;
}

std::vector<float> CVMisc::sideMagnitude(const cv::Rect &rect, const std::vector<std::pair<cv::Point, cv::Point> > &lines, float minThresh)
{

    std::vector<float> magnitudes(4,0.0f);

    float width = (float)rect.width;
    float height = (float)rect.height;

    // first line
    cv::Point2f p11 = cv::Point2f(rect.tl());
    cv::Point2f p12 = cv::Point2f(rect.br().x, rect.tl().y);

    // second line
    cv::Point2f p21 = cv::Point2f(rect.br().x, rect.tl().y);
    cv::Point2f p22 = cv::Point2f(rect.br());

    // third line
    cv::Point2f p31 = cv::Point2f(rect.br());
    cv::Point2f p32 = cv::Point2f(rect.tl().x,rect.br().y);

    // fourth line
    cv::Point2f p41 = cv::Point2f(rect.tl().x,rect.br().y);
    cv::Point2f p42 = cv::Point2f(rect.tl());

    std::vector<cv::Point2f> l1Pts;
    std::vector<cv::Point2f> l2Pts;
    std::vector<cv::Point2f> l3Pts;
    std::vector<cv::Point2f> l4Pts;


    for ( size_t i = 0; i < lines.size(); i++ )
    {
        cv::Point2f pt0 = cv::Point2f(lines[i].first);
        cv::Point2f pt1 = cv::Point2f(lines[i].second);

        cv::Point2f pti1;
        cv::Point2f pti2;
        cv::Point2f pti3;
        cv::Point2f pti4;

        if ( intersection(p11, p12, pt0,pt1, pti1)) l1Pts.push_back(pti1);
        if ( intersection(p21, p22, pt0,pt1, pti2)) l2Pts.push_back(pti2);
        if ( intersection(p31, p32, pt0,pt1, pti3)) l3Pts.push_back(pti3);
        if ( intersection(p41, p42, pt0, pt1, pti4)) l4Pts.push_back(pti4);

    }

    if (l1Pts.size() > 0)
    {
        std::sort( l1Pts.begin(), l1Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.x < rhs.x;
        });


        float dist = cv::norm(l1Pts.at(0) - p11);
        float n = 1;


        for (size_t i = 0; i < l1Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l1Pts.at(i + 1) - l1Pts.at(i));

            if (distTmp/width > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[0] = n*dist;
    }

    if (l2Pts.size() > 0)
    {
        std::sort( l2Pts.begin(), l2Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.y < rhs.y;
        });

        float dist = cv::norm(l2Pts.at(0) - p21);
        float n = 1;

        for (size_t i = 0; i < l2Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l2Pts.at(i + 1) - l2Pts.at(i));

            if (distTmp/height > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[1] = n*dist;
    }

    if (l3Pts.size() > 0)
    {
        std::sort( l3Pts.begin(), l3Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.x > rhs.x;
        });

        float dist = cv::norm(l3Pts.at(0) - p31);
        float n = 1;


        for (size_t i = 0; i < l3Pts.size() - 1; i++)
        {

            float distTmp = (float)cv::norm(l3Pts.at(i + 1) - l3Pts.at(i));

            if (distTmp/width > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[2] = n*dist;
    }

    if (l4Pts.size() > 0)
    {
        std::sort( l4Pts.begin(), l4Pts.end(), []( const cv::Point2f& lhs, const cv::Point2f& rhs )
        {
            return lhs.y > rhs.y;
        });



        float dist = cv::norm(l4Pts.at(0) - p41);
        float n = 1;


        for (size_t i = 0; i < l4Pts.size() - 1; i++)
        {
            float distTmp = (float)cv::norm(l4Pts.at(i + 1) - l4Pts.at(i));

            if (distTmp/height > minThresh)
            {
                dist += distTmp;
                n++;
            }

        }

        magnitudes[3] = n*dist;
    }

    magnitudes[0] = magnitudes[0]/width;
    magnitudes[1] = magnitudes[1]/height;
    magnitudes[2] = magnitudes[2]/width;
    magnitudes[3] = magnitudes[3]/height;

    return magnitudes;
}

std::vector<float> CVMisc::elipticalFeature(const cv::Rect &rect, const std::vector<std::pair<cv::Point, cv::Point> > &lines, cv::RotatedRect &ellipse)
{
    std::vector<cv::Point> pointsInRect;
    std::vector<float> result(4, 0.0f);



    for(const std::pair<cv::Point, cv::Point> &line : lines)
    {
        cv::Point pt0 = line.first;
        cv::Point pt1 = line.second;

        if (pointInRect(rect, pt0))
            pointsInRect.push_back(pt0);


        if (pointInRect(rect, pt1))
            pointsInRect.push_back(pt1);
    }

    if (pointsInRect.size() >= 4)
    {
        cv::RotatedRect minEllipse = cv::minAreaRect(pointsInRect);
        ellipse = minEllipse;

        float angle = minEllipse.angle;

        float min = (float)std::min(minEllipse.size.width, minEllipse.size.height);
        float max = (float)std::max(minEllipse.size.width, minEllipse.size.height);

        float maxMinRatio = max/min;

        float xPos = minEllipse.center.x;
        float yPos = minEllipse.center.y;

        float xRef = (float)(xPos - rect.tl().x)/(float)rect.width;
        float yRef = (float)(yPos - rect.tl().y)/(float)rect.height;

        result[0] = angle;
        result[1] = maxMinRatio;
        result[2] = xRef;
        result[3] = yRef;

    }

    return result;
}

void CVMisc::clusterPairOfPoints(Circle &c0, Circle &c1, std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1)
{
    std::vector<cv::Point> cluster0(contour.size());
    std::vector<cv::Point> cluster1(contour.size());

    int minLen0 = 0;
    int minLen1 = 0;

    cv::Point *contour_ptr = &contour[0];
    int len = contour.size();

    for(int i = 0; i < len; i++)
    {
        cv::Point testPt = *contour_ptr;
        bool atC0 = pointInCircle(c0, testPt);
        bool atC1 = pointInCircle(c1, testPt);

        if (atC0)
        {
            cluster0.insert(cluster0.begin() + minLen0++, testPt);
        }

        else if (atC1)
        {
            cluster1.insert(cluster1.begin() + minLen1++, testPt);
        }

        contour_ptr++;
    }

    //    for(const cv::Point &pt : contour )
    //    {

    //    }

    out0.assign(cluster0.begin(), cluster0.begin() + minLen0);
    out1.assign(cluster1.begin(), cluster1.begin() + minLen1);


}

void CVMisc::clusterPairOfPoints(const cv::Rect &r0, const cv::Rect &r1, const std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1)
{
    if (!out0.empty())
    {
        out0.clear();
    }

    if (!out1.empty())
    {
        out1.clear();
    }

    for(const cv::Point &pt : contour)
    {
        if (pointInRect(r0, pt))
            out0.push_back(pt);

        else if (pointInRect(r1, pt))
        {
            out1.push_back(pt);
        }
    }

}

void CVMisc::clusterPairOfPoints(const std::vector<cv::Point> &r0, const std::vector<cv::Point> &r1, const std::vector<cv::Point> &contour, std::vector<cv::Point> &out0, std::vector<cv::Point> &out1)
{
    if (!out0.empty())
    {
        out0.clear();
    }

    if (!out1.empty())
    {
        out1.clear();
    }

    for(const cv::Point &pt : contour)
    {
        if (pointInRect(r0, pt))
            out0.push_back(pt);

        if (pointInRect(r1, pt))
        {
            out1.push_back(pt);
        }
    }
}

std::pair<std::vector<cv::Point>, std::vector<cv::Point>> CVMisc::extractSubRectPairs(const cv::RotatedRect &rect)
{
    // get two sides first
    // the two shorter distance


    cv::Point2f v[4];
    rect.points(v);

    // we have vertices

    // v0 & v1
    // v1 & v2
    // v2 & v3
    // v3 & v0

    // compare first the first 2 vertices

    cv::Point p0((int)v[0].x,(int)v[0].y);
    cv::Point p1((int)v[1].x,(int)v[1].y);
    cv::Point p2((int)v[2].x,(int)v[2].y);

    double dist0 = dist(p0, p1);
    double dist1 = dist(p1, p2);

    double minDist;
    double maxDist;

    std::pair<cv::Point, cv::Point> mp0;
    std::pair<cv::Point, cv::Point> mp1;

    if (dist0 < dist1)
    {
        mp0 = std::pair<cv::Point,cv::Point>(p0,p1);

        // if v0 & v1 is selected the indexes of the parallel
        // vertices is v2 & v3

        cv::Point pp0((int)v[2].x, (int)v[2].y);
        cv::Point pp1((int)v[3].x, (int)v[3].y);

        mp1 = std::pair<cv::Point,cv::Point>(pp0,pp1);

        minDist = dist0;
        maxDist = dist1;
    }
    else
    {
        mp0 = std::pair<cv::Point,cv::Point>(p1,p2);

        // if v1 & v2 is selected the indexes of the parallel
        // vertices is v3 & v0

        cv::Point pp0((int)v[3].x, (int)v[3].y);
        cv::Point pp1((int)v[0].x, (int)v[0].y);

        mp1 = std::pair<cv::Point,cv::Point>(pp0,pp1);

        minDist = dist1;
        maxDist = dist0;
    }

    float rat = (float)(minDist/maxDist);

    std::vector<cv::Point> rects0;
    std::vector<cv::Point> rects1;

    cv::Point bet_p00 = pointInBet(mp0.second,mp1.first, rat);
    cv::Point bet_p01 = pointInBet(mp0.first,mp1.second, rat);

    rects0.push_back(mp0.first);
    rects0.push_back(mp0.second);
    rects0.push_back(bet_p00);
    rects0.push_back(bet_p01);

    cv::Point bet_p10 = pointInBet(mp0.first,mp1.second, 1.0f - rat);
    cv::Point bet_p11 = pointInBet(mp0.second,mp1.first, 1.0f - rat);

    rects1.push_back(mp1.first);
    rects1.push_back(mp1.second);
    rects1.push_back(bet_p10);
    rects1.push_back(bet_p11);

    return std::pair<std::vector<cv::Point>, std::vector<cv::Point>>(rects0,rects1);
}

float CVMisc::maxDist(std::vector<cv::Point> vertices)
{
    int len = vertices.size();

    double maxDist = 0.0f;

    for (int i = 0; i < len; i++)
    {
        cv::Point pt0(vertices[i]);
        cv::Point pt1(vertices[(i + 1) % len]);

        double tmpDist = dist(pt0,pt1);


        if (maxDist < tmpDist)
            maxDist = tmpDist;
    }

    return (float)maxDist;

}


std::vector<cv::Point> CVMisc::pointGen(const cv::Point &pt0, const cv::Point &pt1, const int &N)
{
    std::vector<cv::Point> result;

    int x0 = pt0.x;
    int y0 = pt0.y;

    int x1 = pt1.x;
    int y1 = pt1.y;

    for(int k = 1; k <= N; k++)
    {
        double kd = k;
        double xDeld = x1 - x0;
        double yDeld = y1 - y0;
        double Nd = N;

        double xd = (kd * xDeld / Nd) + x0;
        double yd = (kd * yDeld / Nd) + y0;

        double xRounded = round(xd);
        double yRounded = round(yd);

        cv::Point pt((int)xRounded, (int)yRounded);

        result.push_back(pt);
    }

    return result;
}

double CVMisc::dist(cv::Point pt0, cv::Point pt1)
{
    double x0 = (double)pt0.x;
    double y0 = (double)pt0.y;

    double x1 = (double)pt1.x;
    double y1 = (double)pt1.y;

    double delX = x1 - x0;
    double delY = y1 - y0;

    double sumSq= delX*delX + delY*delY;

    return sqrt(sumSq);
}

double CVMisc::sum(std::vector<double> nums)
{
    double init = 0.0;
    double sum = std::accumulate(nums.begin(),nums.end(),init);

    return sum;
}


double CVMisc::mean(std::vector<double> nums)
{
    double sumOfVals = CVMisc::sum(nums);

    double n = (double)nums.size();

    return sumOfVals / n;
}

cv::Point CVMisc::centroid(const std::vector<cv::Point> &points)
{
    cv::Moments m = cv::moments(points,false);
    cv::Point2f center(m.m10/m.m00, m.m01/m.m00);

    return cv::Point(center);
}

bool CVMisc::roiValid(const cv::Mat &toBeCropped, const cv::Rect &croppingROI)
{
    bool v0 = 0 <= croppingROI.x && croppingROI.x < toBeCropped.cols;
    bool v1 = 0 <= croppingROI.y && croppingROI.y < toBeCropped.rows;
    bool v2 = croppingROI.x + croppingROI.width < toBeCropped.cols;
    bool v3 = croppingROI.y + croppingROI.height < toBeCropped.rows;

    return v0 && v1 && v2 && v3;

}

float CVMisc::rectIntersectArea(const cv::Rect2d &rect0, const cv::Rect2d &rect1)
{

    cv::Rect2d inter = rect0 & rect1;

    return (float)inter.area();


}

void CVMisc::correctRect(const cv::Mat &toBeCropped, const cv::Rect &inputROI, cv::Rect &correctedROI)
{
    int width0 = inputROI.width;
    int height0 =  inputROI.height;

    int widthTotal = toBeCropped.cols;
    int heightTotal = toBeCropped.rows;


    int posX = inputROI.x;
    int posY = inputROI.y;

    int width1 = width0;
    int height1 = height0;

    if (posX <= 0 )
    {
        posX = 1;
    }

    if (posY <= 0)
    {
        posY = 1;
    }

    if (posX >= widthTotal)
    {
        posX = widthTotal - 1;

    }

    if (posY >= heightTotal)
    {
        posY = heightTotal;
    }

    if ( posX + width0 > widthTotal )
    {
        width1 = widthTotal - posX;
    }

    if ( posY + height0 > heightTotal)
    {
        height1 = heightTotal - posY;
    }


    correctedROI = cv::Rect(posX,posY,width1,height1);

}

void CVMisc::drawKeyLines(cv::Mat &outImg, const std::vector<cv::line_descriptor::KeyLine> &keyLines, const cv::Scalar &color, const int &thickness)
{
    for ( size_t i = 0; i < keyLines.size(); i++ )
    {
        KeyLine kl = keyLines[i];

        if( kl.octave == 0)
        {
            cv::Point pt0 = cv::Point( kl.startPointX, kl.startPointY );
            cv::Point pt1 = cv::Point( kl.endPointX, kl.endPointY );

            cv::line(outImg, pt0, pt1, color, thickness);

        }
    }
}

void CVMisc::drawLines(cv::Mat &outImg, const std::vector<std::pair<cv::Point, cv::Point> > &lines, const cv::Scalar &color, const int &thickness)
{
    for ( size_t i = 0; i < lines.size(); i++ )
    {

        cv::Point pt0 = lines[i].first;
        cv::Point pt1 = lines[i].second;

        cv::line(outImg, pt0, pt1, color, thickness);

    }

}

void CVMisc::drawRotRect(cv::Mat &outImg, const cv::RotatedRect &rect, cv::Scalar color, int thickness)
{
    cv::Point2f rect_points[4];

    rect.points(rect_points);

    for( int i = 0; i < 4; i++ )
        cv::line( outImg, rect_points[i], rect_points[(i+1)%4], color, thickness);

}

void CVMisc::drawConvex(cv::Mat &outImg, std::vector<cv::Point> &points, cv::Scalar color, int thickness)
{
    //    rect.points(rect_points);

    for( int i = 0; i < points.size(); i++ )
        cv::line( outImg, points[i], points[(i+1)%4], color, thickness);

}

void CVMisc::filterKeyLines(const cv::RotatedRect &rect, const std::vector<KeyLine> &keyLines, std::vector<KeyLine> &outKeyLines)
{
    for ( size_t i = 0; i < keyLines.size(); i++ )
    {
        KeyLine kl = keyLines[i];

        if( kl.octave == 0)
        {
            cv::Point pt0 = cv::Point( kl.startPointX, kl.startPointY );
            cv::Point pt1 = cv::Point( kl.endPointX, kl.endPointY );

            bool pt0Valid = pointInRect(rect,pt0);
            bool pt1Valid = pointInRect(rect, pt1);

            if (pt0Valid || pt1Valid)
                outKeyLines.push_back(kl);
        }
    }
}

void CVMisc::filterKeyPoints(const cv::RotatedRect &rect, const std::vector<cv::KeyPoint> &kps, std::vector<cv::KeyPoint> &out_kps)
{
    if (!out_kps.empty())
    {
        out_kps.clear();
    }

    for(const cv::KeyPoint &kp : kps)
    {
        if (pointInRect(rect,cv::Point(kp.pt)))
            out_kps.push_back(kp);
    }
}


cv::Mat CVMisc::cropRotRect(const cv::Mat &src, const cv::RotatedRect &rect)
{
    cv::Mat M, rotated, cropped;
    // get angle and size from the bounding box
    float angle = rect.angle;
    cv::Size rect_size = rect.size;
    // thanks to http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
    if (rect.angle < -45.) {
        angle += 90.0;
        std::swap(rect_size.width, rect_size.height);
    }
    // get the rotation matrix
    M = cv::getRotationMatrix2D(rect.center, angle, 1.0);
    // perform the affine transformation
    cv::warpAffine(src, rotated, M, src.size(), cv::INTER_CUBIC);
    // crop the resulting image
    cv::getRectSubPix(rotated, rect_size, rect.center, cropped);

    return cropped;
}
