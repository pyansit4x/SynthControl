#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T20:55:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SynthControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usb.cpp

HEADERS  += mainwindow.h \
    usb.h

FORMS    += mainwindow.ui

win32: LIBS += -lwinusb

win32: LIBS += -lSetupAPI
