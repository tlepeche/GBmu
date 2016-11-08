
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GBmu
TEMPLATE = app

OBJECTS_DIR   = obj

INCPATH  += include/gb/gui \
			include/db \
			include/gb \
			include/gb/core/ \
			./

SOURCES  += src/main.cpp \
			src/gb/gui/OpenGLWindow.cpp \
			src/db/DbWindow.cpp \
		    src/gb/core/Memory.cpp \
		    src/gb/core/Rom.cpp \
			src/gb/core/Machine.cpp \
			src/gb/core/Cpu.cpp \
			src/gb/core/Timer.cpp \
			src/gb/Gameboy.cpp \
			src/gb/core/CpuOpcode.cpp


HEADERS  += include/gb/gui/OpenGLWindow.hpp \
			include/db/DbWindow.hpp

FORMS    +=  form/dbwindow.ui
