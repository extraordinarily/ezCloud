#-------------------------------------------------
#
# Project created by QtCreator 2017-05-19T17:11:58
#
#-------------------------------------------------

QT       += core gui
QT       +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpServer
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp

HEADERS  += server.h

FORMS    += server.ui
