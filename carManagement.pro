#-------------------------------------------------
#
# Project created by QtCreator 2020-02-18T14:48:07
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = carManagement
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    register_page.cpp \
    management_page.cpp \
    docxml.cpp

HEADERS  += login.h \
    register_page.h \
    management_page.h \
    docxml.h

FORMS    += login.ui \
    management_page.ui

CONFIG += c++11
