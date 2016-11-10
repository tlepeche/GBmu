#include "Machine.hpp"
#include <unistd.h>
#include "registerAddr.hpp"

#define DEBUG_LOOP 1
#define DEBUG_ROM 1
#define DEBUG_GPU 1

/*
** ############################################################################
** PUBLIC Function
** ############################################################################
*/
Machine::Machine(void) : _memory(Memory::Instance()), _clock(Timer::Instance()), _cpu(Cpu_z80::Instance()), _gpu(Gpu::Instance())
{
	this->_cpu.init();
}

bool Machine::step(void)
{
	this->_clock.setFrequency(this->_cpu.getArrayFrequency());
	this->_clock.setCycleTotal(this->_getCycleOpcode());
	if (((this->_memory.read_byte(REGISTER_TAC) & 0x4) == 0x4))
	{
		if (this->_cpu.nbCycleNextOpCode() < this->_clock.getCycleAcc()) {
			unsigned int clock = this->_cpu.executeNextOpcode();

			this->_clock.setCycleAcc(clock);
			this->_gpu.step();
			this->_gpu.accClock(clock);
			return (true);
		}
		else
		{
			//this->_gpu.render();
			this->_clock.sleep(this->_getFrequencyFrameTimeGpu());
			this->_clock.reset();
			this->_cpu.interrupt();
			return (false);
		}
	}
	return (true);
}

void Machine::run(void)
{
	this->step();
	this->run();
}

uint8_t Machine::_getCycleOpcode(void)
{
	double period;
	const unsigned int typeFrequency = this->_memory.read_byte(REGISTER_TAC) & 0x3;

	period = (double) (1. / (float)this->_clock.getArrayFrequency(typeFrequency));
	period *= 1000;
	return (this->_getFrequencyFrameTimeGpu() / period);
}

uint8_t Machine::_getFrequencyFrameTimeGpu(void)
{
	return ((unsigned int)(1000 / 60)); //TODO: CHange 60 by this->_gpu.getFrequency when gpu driver is ok
}
