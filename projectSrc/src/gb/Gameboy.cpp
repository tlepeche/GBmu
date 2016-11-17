

#include "OpenGLWindow.hpp"
#include "DbWindow.hpp"
#include "registerAddr.hpp"
#include "Gameboy.hpp"

void setLowBit(Memory *memory, uint16_t addr, uint8_t bit)
{
	memory->write_byte(addr, (uint8_t)((0x01 << bit) ^ memory->read_byte(addr)), true);
}

void setHightBit(Memory *memory, uint16_t addr, uint8_t bit)
{
	memory->write_byte(addr, (uint8_t)((0x01 << bit) | memory->read_byte(addr)), true);
}

Gameboy::Gameboy() :
	_window(OpenGLWindow::Instance())
	, _windowDebug(nullptr)
	, _thread(nullptr)
	, _romPath("")
{
	this->_hardware = AUTO;
	_stepMode.store(false);
	_willRun.store(false);

	connect(_window, &OpenGLWindow::openRomSign, this, &Gameboy::openRomSlot);
	connect(_window, &OpenGLWindow::gbDbSign, this, &Gameboy::gbDbSlot);
	connect(_window, &OpenGLWindow::keyPressSign, this, &Gameboy::KeyPress);
	connect(_window, &OpenGLWindow::keyReleaseSign, this, &Gameboy::KeyRelease);

	_window->show();
#ifdef DEBUG
	gbDbSlot(); // Open Debug window
	openRomSlot("/sgoinfre/goinfre/Misc/roms/Tetris.gb"); // Run Roms
#endif
}

Gameboy::~Gameboy()
{
	delete this->_windowDebug;
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
	if (isBreakpoint(_cpu->_cpuRegister.PC))
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
		this->_memory->reset();
		if (_memory->loadRom(_romPath.c_str(), this->_hardware))
			_willRun.store(false);
		else
		{
			htype		hardRom;
			hardRom = (this->_hardware == AUTO) ? this->_memory->getRomType() : this->_hardware;
			this->_cpu->init(hardRom);
			this->_gpu->init(); // TODO pour passer hardware au gpu: this->_gpu->init(hardRom)
			_thread = new std::thread(&Gameboy::run, this);
		}
	}
	else
		std::cerr << "Gameboy: No rom path defined" << std::endl;
}

void	Gameboy::stepPressedSlot()
{
	_stepMode.store(true);
	gstep();
}

#include "registerAddr.hpp"
void	Gameboy::framePressedSlot()
{
	_stepMode.store(true);

	uint16_t	start;
	start = _memory->read_byte(REGISTER_LY);
	while (start == _memory->read_byte(REGISTER_LY))
		gstep();
	while (start != _memory->read_byte(REGISTER_LY))
		gstep();
}

void	Gameboy::resetPressedSlot()
{
	_stepMode.store(true);
	reset();
}

void	Gameboy::openRomSlot(std::string path)
{
	_romPath = path;
	reset();
}

void	Gameboy::gbDbSlot()
{
	_windowDebug = new DbWindow(&_cpu->_cpuRegister, _memory, &_breakpoints);
	connect(_windowDebug, &DbWindow::stepPressedSign, this, &Gameboy::stepPressedSlot);
	connect(_windowDebug, &DbWindow::framePressedSign, this, &Gameboy::framePressedSlot);
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


void	Gameboy::KeyPress(int key)
{
//	if ((_memory->read_byte(REGISTER_INPUT) & 0x30) == 0x30)
//	{
//		if (key == UP || key == DOWN || key == LEFT || key == RIGHT)
//			setLowBit(_memory, REGISTER_INPUT, 4);
//		else
//			setLowBit(_memory, REGISTER_INPUT, 5);
//	}
	switch(key)
	{
		case RIGHT:
		case A_BUTTON:
			setLowBit(_memory, REGISTER_INPUT, 0);
			break;
		case LEFT:
		case B_BUTTON:
			setLowBit(_memory, REGISTER_INPUT, 1);
			break;
		case UP:
		case SELECT:
			setLowBit(_memory, REGISTER_INPUT, 2);
			break;
		case DOWN:
		case START:
			setLowBit(_memory, REGISTER_INPUT, 3);
			break;
	}
}

void	Gameboy::KeyRelease(int key)
{
	switch(key)
	{
		case RIGHT:
		case A_BUTTON:
			setHightBit(_memory, REGISTER_INPUT, 0);
			break;
		case LEFT:
		case B_BUTTON:
			setHightBit(_memory, REGISTER_INPUT, 1);
			break;
		case UP:
		case SELECT:
			setHightBit(_memory, REGISTER_INPUT, 2);
			break;
		case DOWN:
		case START:
			setHightBit(_memory, REGISTER_INPUT, 3);
			break;
	}
}

