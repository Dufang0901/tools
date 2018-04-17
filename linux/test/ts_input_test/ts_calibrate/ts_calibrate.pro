#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T10:02:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ts_calibrate
TEMPLATE = app


SOURCES += main.cpp\
    HteProtocol.c \
    C_RecvCanThread.cpp \
    C_PadCalibrateForm.cpp

HEADERS  += \
    HteProtocol.h \
    C_RecvCanThread.h \
    C_PadCalibrateForm.h

FORMS    += \
    C_PadCalibrateForm.ui
