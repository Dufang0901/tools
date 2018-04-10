#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T10:02:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testMouse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    HteProtocol.c \
    C_RecvCanThread.cpp \
    form.cpp \
    Rtc.c \
    Rtc_RX8025.c

HEADERS  += mainwindow.h \
    HteProtocol.h \
    C_RecvCanThread.h \
    form.h \
    Rtc_RX8025.h \
    Rtc.h

FORMS    += mainwindow.ui \
    form.ui
