#-------------------------------------------------
#
# Project created by QtCreator 2015-07-23T22:47:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtMagicSearch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    httpconnector.cpp \
    card.cpp

HEADERS  += mainwindow.h \
    card.h \
    controller.h \
    httpconnector.h \
    constants.h

FORMS    += mainwindow.ui
