#-------------------------------------------------
#
# Project created by QtCreator 2017-11-23T14:59:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrimeScene_beta
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    maincontroller.cpp \
    pointcloudloader.cpp \
    pointcloud.cpp \
    scene.cpp \
    sceneviewer.cpp \
    airphoto.cpp \
    hotspot.cpp \
    hotspotloader.cpp \
    octreenode.cpp \
    octreebuilder.cpp \
    roamroute.cpp \
    scenemode.cpp

HEADERS += \
    mainwindow.h \
    maincontroller.h \
    pointcloudloader.h \
    pointcloud.h \
    scene.h \
    sceneviewer.h \
    airphoto.h \
    hotspot.h \
    hotspotloader.h \
    octreenode.h \
    octreebuilder.h \
    roamroute.h \
    scenemode.h \
    sceneglobal.h

FORMS += \
    mainwindow.ui \
    scenecontrollerwindow.ui \
    hotspot.ui

#glfw glew glm opengl
INCLUDEPATH += \
    "$$PWD/qt_extends/include/"
DEPENDPATH += \
    "$$PWD/qt_extends/lib-mingw/glfw/" \
    "$$PWD/qt_extends/lib-mingw/glew/1"
LIBS += -L"$$PWD/qt_extends/lib-mingw/glfw/" -lglfw3dll
LIBS += -L"$$PWD/qt_extends/lib-mingw/glew/1" -lglew32.dll
LIBS += -lopengl32
LIBS += -lglu32
