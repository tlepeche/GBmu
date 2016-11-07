

#include "Gameboy.hpp"


Gameboy::Gameboy() :
	_window(new OpenGLWindow()),
	is_debug_mode(true)
{

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
	if (is_debug_mode)
	{

	}
	else
	{
		step();
	}
	sleep(3);
	run();
}

void	Gameboy::openRomSlot(std::string path)
{
	std::cout << "Gameboy path: " << path << std::endl;
}
