#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T22:11:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSrc
TEMPLATE = app

INCLUDEPATH += $$PWD/../../../boost/
LIBS += -L$$PWD/../../../boost/stage/lib/

# req'd for OSX
QMAKE_CXXFLAGS += -stdlib=libc++
CONFIG += c++14

LIBS += -lboost_system
LIBS += -lboost_filesystem

SOURCES += main.cpp\
    filesystem.cpp \
    QSrcWnd.cpp

HEADERS  += \
    filesystem.h \
    QSrcWnd.h

FORMS    += mainwindow.ui

DISTFILES += \
    todo.txt
