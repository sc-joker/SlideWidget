#-------------------------------------------------
#
# Project created by QtCreator 2019-03-27T15:45:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageTest
TEMPLATE = app


SOURCES += main.cpp\
		MainWindow.cpp \
    SlideWidget.cpp

HEADERS  += MainWindow.h \
    SlideWidget.h

FORMS    += MainWindow.ui \
    SlideWidget.ui

RESOURCES += \
    pictures.qrc

lessThan(QT_MAJOR_VERSION, 5): DEFINES += ARM
