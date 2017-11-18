#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T12:29:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++11
TARGET = visual_asl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cam_opencv/cvcamplayer.cpp \
    clicklabel.cpp \
    imagecapture.cpp \
    mat_cvt/mat_converter.cpp \
    geometrycompute.cpp \
    sqlite_man/datamanager.cpp \
    data_manager/filemanager.cpp \
    skin_detect/skindetect.cpp \
    compute/compute.cpp \
    compute/handloc.cpp \
    hand_sign_collections.cpp \
    handarmtrainer.cpp \
    classifiermanager.cpp \
    featureextract.cpp

HEADERS  += mainwindow.h \
    cam_opencv/cam_opencv_global.h \
    cam_opencv/cvcamplayer.h \
    core_header.h \
    clicklabel.h \
    imagecapture.h \
    mat_cvt/mat_converter.h \
    geometrycompute.h \
    sqlite_man/datamanager.h \
    data_manager/filemanager.h \
    skin_detect/skindetect.h \
    compute/compute.h \
    compute/handloc.h \
    hand_sign_collections.h \
    handarmtrainer.h \
    classifiermanager.h \
    featureextract.h

FORMS    += mainwindow.ui \
    imagecapture.ui \
    handarmtrainer.ui

SUBDIRS += \
    cam_opencv/cam_opencv.pro \
    cam_opencv/cam_opencv.pro


INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_xobjdetect -lopencv_xfeatures2d -lopencv_line_descriptor -lopencv_tracking

DISTFILES += \
    gest_recog/readme.txt \
    hand_loc/readme.txt \
    skin_detect/readme.txt \
    sqlite_man/readme.txt \
    data_manager/readme.txt \
    compute/readme.txt


