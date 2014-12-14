#-------------------------------------------------
#
# Project created by QtCreator 2014-12-14T21:09:11
#
#-------------------------------------------------

QT       += qml testlib

TARGET = test
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    ../qmlrequire/environment.cpp \
    ../qmlrequire/module.cpp \
    ../qmlrequire/resolve.cpp \
    testrequire.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../qmlrequire/environment.h \
    ../qmlrequire/module.h \
    ../qmlrequire/resolve.h
