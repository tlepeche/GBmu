#include "Machine.hpp"
#include "registerAddr.hpp"
#include "interrupt.hpp"
#include <unistd.h>

#include "Cpu.hpp"
#include "Gpu.hpp"
#include "Memory.hpp"
#include "Timer.hpp"
#include "Audio.hpp"

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU

htype	Machine::_hardware = AUTO;

/*
** ############################################################################
** PUBLIC Function
** ############################################################################
*/
Machine::Machine(void) :
	_memory(new Memory()),
	_cyclesMax(Cpu_z80::getClockSpeed()),
	_cyclesAcc(0)
{
	_audio = new Audio(); _memory->setAudio(_audio);
	_cpu = new Cpu_z80(_memory);
	_gpu = new Gpu(_memory);
	_clock = new Timer(_memory);
}

bool Machine::step(void)
{
	if (_cpu->getStop() == false)
	{
		uint8_t cycles = 0;
		cycles = _cpu->getHalt() ? 4 : _cpu->executeNextOpcode();
		if (_cpu->isInterrupt()) {
			_cpu->execInterrupt();
			cycles = 16;
		}
		_cyclesAcc += cycles;
		_clock->step(cycles);
		cycles >>= (_cpu->isGBCSpeed() ? 1 : 0);
		_gpu->accClock(cycles);
		_gpu->step();
		_audio->step(cycles);
		if (_cyclesAcc >= (uint32_t)(_cyclesMax / 59.7))
		{
			_cyclesAcc -= (uint32_t)(_cyclesMax / 59.7);
		}

		if ((_memory->read_byte(0xFF02) & 0x80) && (_memory->read_byte(0xFF02) & 0x7f) > 0)
		{
			_memory->write_byte(0xFF02, 0);
			_memory->write_byte(REGISTER_IF, _memory->read_byte(REGISTER_IF) | INTER_TIOE);
		}
		if (_cpu->_cpuRegister.PC == 0x0100) // load Rom
			_memory->setInBios(false);
		return (true);
	}
	return (false);
}

void Machine::run(void)
{
	std::cout << "Actually i won't be called" << std::endl;
	while (true)
		this->step();
}

void Machine::setHardware(htype hardware)
{
	_hardware = hardware;
	reset();
}
