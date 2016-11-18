#include "Machine.hpp"
#include "registerAddr.hpp"
#include <unistd.h>

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU

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
	this->_cpu->execInterrupt();
	if (!this->_cpu->getHalt() && !this->_cpu->getStop() && ((this->_memory->read_byte(REGISTER_TAC) & 0x4) == 0x4))
	{
		unsigned int cycles = this->_cpu->executeNextOpcode();
		_cyclesAcc += cycles;

		this->_clock->step(cycles);
		this->_gpu->step();
		this->_gpu->accClock(cycles);
		this->_cpu->execInterrupt();

		if (_cyclesAcc >= _cyclesMax / 60)
		{
			_cyclesAcc -= _cyclesMax / 60;
			usleep(16);
		}
	}
	return (true);
}

void Machine::run(void)
{
	this->step();
	this->run();
}

