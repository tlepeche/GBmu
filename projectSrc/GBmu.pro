
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GBmu
TEMPLATE = app

OBJECTS_DIR   = obj

INCPATH  += include/gb/gui

SOURCES  += src/main.cpp \
			src/gb/gui/openglwindow.cpp 


HEADERS  += include/gb/gui/openglwindow.h

FORMS    +=  
