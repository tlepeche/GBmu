
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GBmu
TEMPLATE = app

OBJECTS_DIR   = obj

LIBS += -L ~/.brew/lib -lSDL2main -lSDL2

INCPATH  += include/gb/gui \
			include/db \
			include/gb \
			include/gb/core/ \
			$(HOME)/.brew/include \
			src/

SOURCES  += src/main.cpp \
			src/gb/gui/OpenGLWindow.cpp \
			src/db/DbWindow.cpp \
		    src/gb/core/Memory.cpp \
		    src/gb/core/Rom.cpp \
			src/gb/core/Machine.cpp \
			src/gb/core/Cpu.cpp \
			src/gb/core/Gpu.cpp \
			src/gb/core/Timer.cpp \
			src/gb/Gameboy.cpp \
			src/gb/core/CpuOpcode.cpp \
			src/gb/core/CpuCbOpcode.cpp \
			src/gb/core/Bios.cpp \
			src/gb/core/Audio.cpp \
			src/audio/Blip_Buffer.cpp \
			src/audio/Effects_Buffer.cpp \
			src/audio/Gb_Apu.cpp \
			src/audio/Gb_Apu_State.cpp \
			src/audio/Gb_Oscs.cpp \
			src/audio/Multi_Buffer.cpp \
			src/audio/Sound_Queue.cpp

HEADERS  += include/gb/gui/OpenGLWindow.hpp \
			include/db/DbWindow.hpp

FORMS    +=  form/dbwindow.ui
