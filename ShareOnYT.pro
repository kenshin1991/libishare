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
    YouTube/YouTubeUploader.cpp \
    YouTube/YouTubeService.cpp \
    YouTube/YouTubeAuthenticator.cpp
HEADERS += mainwindow.h \
    YouTube/YouTubeUploader.h \
    YouTube/YouTubeService.h \
    YouTube/YouTubeServiceStates.h \
    YouTube/YouTubeAuthenticator.h
FORMS += mainwindow.ui

CONFIG += debug
OBJECTS_DIR = build
MOC_DIR = build/moc
UI_DIR = build/ui
