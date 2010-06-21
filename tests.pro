# Tests for libishare
# Author: Rohit Yadav <rohityadav89 AT gmail.com>

QT += network \
    svg \
    xml

TARGET = libishare-test
TEMPLATE = app

SOURCES +=  test/main.cpp \
            test/mainwindow.cpp \
            src/UploaderIODevice.cpp \
            src/YouTube/YouTubeUploader.cpp \
            src/YouTube/YouTubeService.cpp \
            src/YouTube/YouTubeAuthenticator.cpp \
            test/Gui/ShareOnInternet.cpp

HEADERS +=  test/mainwindow.h \
            src/UploaderIODevice.h \
            src/YouTube/YouTubeUploader.h \
            src/YouTube/YouTubeService.h \
            src/YouTube/YouTubeAuthenticator.h \
            src/YouTube/YouTubeCommon.h \
            test/Gui/ShareOnInternet.h

FORMS += test/mainwindow.ui \
         test/Gui/ShareOnInternet.ui

INCLUDEPATH += src \
               test
CONFIG += debug
OBJECTS_DIR = build
MOC_DIR = build/moc
UI_DIR = build/ui
