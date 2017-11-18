#ifndef CORE_HEADER_H
#define CORE_HEADER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QMouseEvent>

#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QStandardPaths>

#include <QtSql>

#include "cam_opencv/cvcamplayer.h"
#include "mat_cvt/mat_converter.h"
#include "sqlite_man/datamanager.h"
#include "data_manager/filemanager.h"
#include "skin_detect/skindetect.h"
#include "compute/compute.h"

#include "classifiermanager.h"

#include "clicklabel.h"
#include "imagecapture.h"
#include "geometrycompute.h"
#include "featureextract.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/ml.hpp"
//#include "opencv2/tracking.hpp"

// From opencv_contrib or OpenCV Extra Modules
#include "opencv2/xobjdetect.hpp"
#include "opencv2/tracking.hpp"
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>


#endif // CORE_HEADER_H
