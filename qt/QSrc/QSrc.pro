#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T22:11:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSrc
TEMPLATE = app

INCLUDEPATH += C:/sw/boost/
LIBS += "-LC:/sw/boost/stage/lib/"


SOURCES += main.cpp\
        mainwindow.cpp \
    filesystem.cpp

HEADERS  += mainwindow.h \
    filesystem.h

FORMS    += mainwindow.ui
