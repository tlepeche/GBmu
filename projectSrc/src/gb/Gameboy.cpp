

#include "OpenGLWindow.hpp"
#include "DbWindow.hpp"
#include "Gameboy.hpp"

Gameboy::Gameboy() :
	_window(new OpenGLWindow())
	, _windowDebug(nullptr)
	, _thread(nullptr)
	, _romPath("")
{
	_debugMode.store(false);
	_willRun.store(false);

	connect(_window, &OpenGLWindow::openRomSign, this, &Gameboy::openRomSlot);
	connect(_window, &OpenGLWindow::gbDbSign, this, &Gameboy::gbDbSlot);

	_window->show();
#ifdef DEBUG
	gbDbSlot(); // Open Debug window
	openRomSlot("/sgoinfre/goinfre/Misc/roms/Tetris.gb"); // Run Roms
#endif
}

Gameboy::~Gameboy()
{
	delete this->_windowDebug;
	delete this->_window;
	this->stopThread();
}

void	Gameboy::stopThread()
{
	_willRun.store(false);
	_thread->join();
	delete _thread;
}

void	Gameboy::run()
{
	if (_willRun)
	{
		if (_debugMode && _windowDebug)
		{
			// if freeRun
			// 	step();
		}
		else
		{
			step();
		}
		run();
	}
}

void	Gameboy::reset()
{
	if (_willRun.load())
	{
		stopThread();
	}

	if (_romPath.length())
	{
		_willRun.store(true);
		this->_memory.reset();
		Rom::Instance().load(_romPath.c_str());
		this->_cpu.init();
		_thread = new std::thread(&Gameboy::run, this);
	}
	else
		std::cerr << "Gameboy: No rom path defined" << std::endl;
}

void	Gameboy::stepPressedSlot()
{
	step();
}

void	Gameboy::resetPressedSlot()
{
	reset();
}

void	Gameboy::openRomSlot(std::string path)
{
	_romPath = path;
	reset();
}

void	Gameboy::gbDbSlot()
{
	_windowDebug = new DbWindow(&_cpu._cpuRegister, &_memory);
	connect(_windowDebug, &DbWindow::stepPressedSign, this, &Gameboy::stepPressedSlot);
	connect(_windowDebug, &DbWindow::resetPressedSign, this, &Gameboy::resetPressedSlot);

	_windowDebug->show();
	_debugMode.store(true);
}
