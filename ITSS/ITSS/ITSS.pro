#-------------------------------------------------
#
# Project created by QtCreator 2017-09-28T17:20:23
#
#-------------------------------------------------
QT       += charts
QT       += core gui
QT       += sql
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ITSS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    etccharge.cpp \
    welcome.cpp \
    etccarinfo.cpp \
    etcrecharge.cpp \
    parksites.cpp \
    etcrundialog.cpp \
    parkuserinfo.cpp \
    parkcharge.cpp \
    parkrundialog.cpp \
    dialogs/etccarinfoadddialog.cpp \
    dialogs/etccarinfoupdatedialog.cpp \
    dialogs/parkuserinfoadddialog.cpp \
    dialogs/parkuserinfoupdatedialog.cpp \
    trafficlightmanage.cpp \
    trafficviolationrecord.cpp \
    trafficflow.cpp \
    dialogs/lightinfoadddialog.cpp \
    trafficviolationdialog.cpp \
    MyUtils/loginusrsingleton.cpp \
    dialogs/logindialog.cpp \
    dialogs/registerdialog.cpp \
    aboutusdialog.cpp

HEADERS  += mainwindow.h \
    etccharge.h \
    welcome.h \
    etccarinfo.h \
    etcrecharge.h \
    parksites.h \
    etcrundialog.h \
    parkuserinfo.h \
    parkcharge.h \
    parkrundialog.h \
    variabledef.h \
    dialogs/etccarinfoadddialog.h \
    dialogs/etccarinfoupdatedialog.h \
    dialogs/parkuserinfoadddialog.h \
    dialogs/parkuserinfoupdatedialog.h \
    trafficlightmanage.h \
    trafficviolationrecord.h \
    trafficflow.h \
    dialogs/lightinfoadddialog.h \
    trafficviolationdialog.h \
    MyUtils/loginusrsingleton.h \
    dialogs/logindialog.h \
    dialogs/registerdialog.h \
    aboutusdialog.h

FORMS    += mainwindow.ui \
    etccharge.ui \
    welcome.ui \
    etccarinfo.ui \
    etcrecharge.ui \
    parksites.ui \
    etcrundialog.ui \
    parkuserinfo.ui \
    parkcharge.ui \
    parkrundialog.ui \
    dialogs/etccarinfoadddialog.ui \
    dialogs/etccarinfoupdatedialog.ui \
    dialogs/parkuserinfoadddialog.ui \
    dialogs/parkuserinfoupdatedialog.ui \
    trafficlightmanage.ui \
    trafficviolationrecord.ui \
    trafficflow.ui \
    dialogs/lightinfoadddialog.ui \
    trafficviolationdialog.ui \
    dialogs/logindialog.ui \
    dialogs/registerdialog.ui \
    aboutusdialog.ui

RESOURCES += \
    images/myimages.qrc

DISTFILES += \
    sqlcode
