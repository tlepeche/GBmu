

#include "Gameboy.hpp"

Gameboy::Gameboy() :
	_window(new OpenGLWindow())
	, is_debug_mode(false)
	, _thread(nullptr)
{
	_willRun.store(false);

	connect(_window, &OpenGLWindow::openRomSign, this, &Gameboy::openRomSlot);
	_window->show();
}

Gameboy::~Gameboy()
{

	delete this->_window;
}

#include <ctime>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

void	Gameboy::run()
{
	if (_willRun)
	{
		if (is_debug_mode)
		{

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
