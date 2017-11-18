#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T12:25:19
#
#-------------------------------------------------

QT       += sql

TARGET = cam_opencv
TEMPLATE = lib

DEFINES += CAM_OPENCV_LIBRARY

SOURCES += cvcamplayer.cpp

HEADERS += cvcamplayer.h\
        cam_opencv_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_viz

