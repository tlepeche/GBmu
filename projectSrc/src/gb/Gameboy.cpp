

#include "OpenGLWindow.hpp"
#include "DbWindow.hpp"
#include "Gameboy.hpp"

Gameboy::Gameboy() :
	_window(new OpenGLWindow())
	, _windowDebug(nullptr)
	, _thread(nullptr)
{
	_debugMode.store(false);
	_willRun.store(false);

	connect(_window, &OpenGLWindow::openRomSign, this, &Gameboy::openRomSlot);
	connect(_window, &OpenGLWindow::gbDbSign, this, &Gameboy::gbDbSlot);

	_window->show();
}

Gameboy::~Gameboy()
{
	delete this->_windowDebug;
	delete this->_window;
}
#include <unistd.h>

void	Gameboy::run()
{
	if (_willRun)
	{
		if (_debugMode && _windowDebug)
		{
			step();
		}
		else
		{
			step();
		}
		std::cout << "STEP" << std::endl;
		sleep(1);
		run();
	}
}

void	Gameboy::openRomSlot(std::string path)
{
	std::cout << "Gameboy path: " << path << std::endl;
	_willRun.store(true);
	_thread = new std::thread(&Gameboy::run, this);
}

void	Gameboy::gbDbSlot()
{
	_windowDebug = new DbWindow(&_cpu._cpuRegister, &_memory);
	_windowDebug->show();
	_debugMode.store(true);
}
