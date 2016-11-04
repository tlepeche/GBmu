
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GBmu
TEMPLATE = app

OBJECTS_DIR   = obj

INCPATH  += include/gb/gui \
			include/db \
			include/gb/core/

SOURCES  += src/main.cpp \
			src/gb/gui/openglwindow.cpp \
			src/db/dbwindow.cpp \
		    src/gb/core/Memory.cpp	


HEADERS  += include/gb/gui/openglwindow.h \
			include/db/dbwindow.h

FORMS    +=  form/dbwindow.ui
