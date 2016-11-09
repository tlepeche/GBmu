

#include "OpenGLWindow.hpp"
#include "DbWindow.hpp"
#include "Gameboy.hpp"

Gameboy::Gameboy() :
	_window(new OpenGLWindow())
	, _windowDebug(nullptr)
	, _thread(nullptr)
	, _romPath("")
{
	_stepMode.store(false);
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

void	Gameboy::gstep()
{
	if (!step()) step();
	if (isBreakpoint(_cpu._cpuRegister.PC))
		_stepMode.store(true);
}

void	Gameboy::run()
{
	if (_willRun)
	{
		if (_stepMode)
		{
		}
		else
		{
			gstep();
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
	_stepMode.store(true);
	gstep();
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
	_windowDebug = new DbWindow(&_cpu._cpuRegister, &_memory, &_breakpoints);
	connect(_windowDebug, &DbWindow::stepPressedSign, this, &Gameboy::stepPressedSlot);
	connect(_windowDebug, &DbWindow::runPressedSign, this, &Gameboy::switchStepModeSlot);
	connect(_windowDebug, &DbWindow::resetPressedSign, this, &Gameboy::resetPressedSlot);
	connect(_windowDebug, &DbWindow::openPressedSign, this, &Gameboy::openRomSlot);

	connect(_windowDebug, &DbWindow::bpAddSign, this, &Gameboy::addBreakpointSlot);
	connect(_windowDebug, &DbWindow::bpDelSign, this, &Gameboy::delBreakpointSlot);

	_windowDebug->show();
	_stepMode.store(true);
}

void	Gameboy::switchStepModeSlot(void)
{
	_stepMode.store(!_stepMode.load());
}

void	Gameboy::addBreakpointSlot(uint16_t addr)
{
	auto it = std::find(_breakpoints.begin(), _breakpoints.end(), addr);

	if (it == _breakpoints.end())
		_breakpoints.push_back(addr);
}

void	Gameboy::delBreakpointSlot(uint16_t addr)
{
	_breakpoints.remove(addr);
}

bool	Gameboy::isBreakpoint(uint16_t addr)
{
	auto it = std::find(_breakpoints.begin(), _breakpoints.end(), addr);

	return (it != _breakpoints.end());
}
