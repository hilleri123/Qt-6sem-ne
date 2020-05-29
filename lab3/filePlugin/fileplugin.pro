TEMPLATE = lib
TARGET = filePlugin
QT -= gui
QT += concurrent
CONFIG += plugin c++11

DEFINES += MYPLUGIN_LIBRARY

TARGET = $$qtLibraryTarget($$TARGET)

# Input
SOURCES += \
    fileplugin.cpp

HEADERS += \
    fileplugin.h \
    plugin.h \
    myplugin_global.h

SUBDIRS += \
    fileplugin.pro
