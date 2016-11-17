#include "Machine.hpp"
#include <unistd.h>
#include "registerAddr.hpp"

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU

/*
** ############################################################################
** PUBLIC Function
** ############################################################################
*/
Machine::Machine(void) :
	_memory(new Memory())
{
	_cpu = new Cpu_z80(_memory);
	_gpu = new Gpu(_memory);
	_clock = new Timer(_memory);
	this->_cpu->init();
}

bool Machine::step(void)
{
	this->_clock->setFrequency(this->_cpu->getArrayFrequency());
	this->_clock->setCycleTotal(this->_getCycleOpcode());
	if ((this->_cpu->getIME() || (!this->_cpu->getIME() && this->_cpu->getHalt()) || (!this->_cpu->getIME() && this->_cpu->getStop())) && this->_cpu->isInterrupt())
		this->_cpu->execInterrupt();
	if (!this->_cpu->getHalt() && !this->_cpu->getStop() && ((this->_memory->read_byte(REGISTER_TAC) & 0x4) == 0x4))
	{
		if (this->_clock->isCycleAcc(this->_cpu->nbCycleNextOpCode())) {
			unsigned int clock = this->_cpu->executeNextOpcode();
			this->_clock->setCycleAcc(clock);
			this->_gpu->step();
			this->_gpu->accClock(clock);
			if (this->_cpu->getIME() && this->_cpu->isInterrupt())
				this->_cpu->execInterrupt();
			return (true);
		}
	}
	return (true);
}

void Machine::run(void)
{
	this->step();
	this->run();
}

uint32_t Machine::_getCycleOpcode(void)
{
	double period;
	const unsigned int typeFrequency = this->_memory->read_byte(REGISTER_TAC) & 0x3;

	period = (double) (1. / (float)this->_clock->getArrayFrequency(typeFrequency));
	period *= 1000;
	return (this->_cpu->getClockSpeed() / this->_clock->getArrayFrequency(typeFrequency));
	return (this->_getFrequencyFrameTimeGpu() / period);
}

uint8_t Machine::_getFrequencyFrameTimeGpu(void)
{
	return ((unsigned int)(1000 / 60)); //TODO: CHange 60 by this->_gpu->getFrequency when gpu driver is ok
}
