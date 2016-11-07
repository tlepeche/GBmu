#ifndef _MACHINE_HPP
#define _MACHINE_HPP

#include "Cpu.hpp"
#include "Memory.hpp"
#include "Timer.hpp"

class Machine
{
	private:
		Timer &_clock;
		Cpu_z80 &_cpu;
		Memory &_memory;
		//Gpu::Gpu &_gpu;


		uint8_t _getFrequencyFrameTimeGpu(void);
		uint8_t _getCycleOpcode(void);

	public:
		Machine(void);
		virtual ~Machine(void) {};
		void run(void);

};
#endif
