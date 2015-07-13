#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T14:56:18
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Schedule
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    task/taskset.cpp \
    task/task.cpp \
    algorithms/annealing.cpp \
    algorithms/weed.cpp

HEADERS  += mainwindow.h \
    task/taskset.h \
    task/task.h \
    algorithms/annealing.h \
    algorithms/weed.h

FORMS    += mainwindow.ui
