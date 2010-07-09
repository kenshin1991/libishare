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
            src/YouTube/YouTubeService.cpp \
            src/YouTube/YouTubeAuthenticator.cpp \
            src/YouTube/YouTubeUploader.cpp \
            src/YouTube/YouTubeFeedParser.cpp \
            src/Vimeo/VimeoService.cpp \
            src/Vimeo/VimeoAuthenticator.cpp \
            src/Gui/ShareOnInternet.cpp

HEADERS +=  test/mainwindow.h \
            src/UploaderIODevice.h \
            src/AbstractSharingService.h \
            src/YouTube/YouTubeService.h \
            src/YouTube/YouTubeAuthenticator.h \
            src/YouTube/YouTubeUploader.h \
            src/YouTube/YouTubeFeedParser.h \
            src/YouTube/YouTubeCommon.h \
            src/Vimeo/VimeoService.h \
            src/Vimeo/VimeoAuthenticator.h \
            src/Gui/ShareOnInternet.h

FORMS += test/mainwindow.ui \
         src/Gui/ShareOnInternet.ui

INCLUDEPATH += src \
               test
CONFIG += debug
OBJECTS_DIR = build
MOC_DIR = build/moc
UI_DIR = build/ui
