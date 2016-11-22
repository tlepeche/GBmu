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
	if (!this->_cpu->getHalt() && !this->_cpu->getStop() && _cyclesAcc < (uint32_t)(_cyclesMax / 59.7))
	{
		unsigned int cycles = this->_cpu->executeNextOpcode();
		_cyclesAcc += cycles;
		this->_clock->step(cycles);
		this->_gpu->accClock(cycles);
		this->_gpu->step();
		if (this->_cpu->getPCRegister() >= 0x100)
			this->_cpu->execInterrupt();
	}
	else if (_cyclesAcc >= (uint32_t)(_cyclesMax / 59.7))
	{
		_cyclesAcc = 0;
		//usleep(16750);
		this->_cpu->execInterrupt();
	}
	else
	{
		if (this->_cpu->getPCRegister() >= 0x100)
			this->_cpu->execInterrupt();
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
