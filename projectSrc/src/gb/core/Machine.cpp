#include "Machine.hpp"
#include "registerAddr.hpp"
#include "interrupt.hpp"
#include <unistd.h>

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
	_cpu = new Cpu_z80(_memory);
	_gpu = new Gpu(_memory);
	_clock = new Timer(_memory);
	this->_cpu->init();
}

bool Machine::step(void)
{
	if (_cyclesAcc < (_cyclesMax / 60))
	{
		unsigned int cycles = this->_cpu->executeNextOpcode();
		_cyclesAcc += cycles;
		this->_clock->step(cycles);
		this->_gpu->accClock(cycles);
		this->_gpu->step();
		this->_cpu->execInterrupt();
	}
	else
	{
		_cyclesAcc = 0;
		_memory->write_byte(REGISTER_IF, _memory->read_byte(REGISTER_IF) | INTER_VBLANK);
		usleep(16750);
	}
	return (true);
}

void Machine::run(void)
{
	this->step();
	this->run();
}

void Machine::setHardware(htype hardware)
{
	_hardware = hardware;
}
