# -------------------------------------------------
# Project created by QtCreator 2010-06-04T19:36:17
# -------------------------------------------------
QT += network \
    svg \
    xml
TARGET = YTApp
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    YouTube/YouTubeUpload.cpp \
    YouTube/YouTubeService.cpp \
    YouTube/YouTubeAuthentication.cpp
HEADERS += mainwindow.h \
    YouTube/YouTubeUpload.h \
    #YouTube/YouTubeService.h \
    YouTube/YouTubeServiceStates.h \
    YouTube/YouTubeAuthentication.h
FORMS += mainwindow.ui

CONFIG += debug
OBJECTS_DIR = build
MOC_DIR = build/moc
UI_DIR = build/ui
