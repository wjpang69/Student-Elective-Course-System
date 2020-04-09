#-------------------------------------------------
#
# Project created by QtCreator 2019-04-08T22:03:00
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SECS
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    enter_admin.cpp \
    stumanage.cpp \
    coursemanage.cpp \
    enter_stu.cpp \
    datastatistics.cpp

HEADERS  += login.h \
    enter_admin.h \
    stumanage.h \
    coursemanage.h \
    enter_stu.h \
    datastatistics.h

FORMS    += login.ui \
    enter_admin.ui \
    stumanage.ui \
    coursemanage.ui \
    enter_stu.ui \
    datastatistics.ui

RESOURCES += \
    images.qrc
