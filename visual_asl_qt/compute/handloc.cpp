#include "handloc.h"

HandLoc::HandLoc()
{

}

std::vector<double> HandLoc::features(std::vector<cv::Point> contour, cv::RotatedRect rect, int n)
{
    std::vector<double> result(n);
    std::vector<double> distances(contour.size() - 1);

    for(int i = 0; i < contour.size() - 1; i++)
    {
        double dist = CVMisc::dist(contour[i], contour[i + 1]);
        distances.insert(distances.begin() + i, dist);
    }





}
