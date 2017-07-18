#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T23:47:03
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myBot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aimlparser.cpp \
    qstartdialog.cpp

HEADERS  += mainwindow.h \
    aimlparser.h \
    qstartdialog.h

FORMS    += mainwindow.ui \
    qstartdialog.ui
