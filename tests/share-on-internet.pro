# 
# Tests for libishare
# Author: Rohit Yadav <rohityadav89 AT gmail.com>
#

QT += network \
    svg \
    xml
TARGET = share-on-internet
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    ../src/UploaderIODevice.cpp \
    ../src/YouTube/YouTubeUploader.cpp \
    ../src/YouTube/YouTubeService.cpp \
    ../src/YouTube/YouTubeAuthenticator.cpp
HEADERS += mainwindow.h \
    ../src/UploaderIODevice.h \
    ../src/YouTube/YouTubeUploader.h \
    ../src/YouTube/YouTubeService.h \
    ../src/YouTube/YouTubeServiceStates.h \
    ../src/YouTube/YouTubeAuthenticator.h
FORMS += mainwindow.ui

INCLUDEPATH += ../src \
               ../src/YouTube

CONFIG += debug
OBJECTS_DIR = ../build
MOC_DIR = ../build/moc
UI_DIR = ../build/ui
