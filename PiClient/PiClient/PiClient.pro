#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T08:24:55
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    posix_qextserialport.cpp \
    qextserialbase.cpp

HEADERS  += mainwindow.h \
    posix_qextserialport.h \
    qextserialbase.h

FORMS    += mainwindow.ui

LIBS += -lwiringPi
