#ifndef _MACHINE_HPP
#define _MACHINE_HPP

#include "Cpu.hpp"
#include "Gpu.hpp"
#include "Memory.hpp"
#include "Timer.hpp"

class Machine
{
	protected:
		Memory	*_memory;
		Timer	*_clock;
		Cpu_z80	*_cpu;
		Gpu		*_gpu;

		uint8_t _getFrequencyFrameTimeGpu(void);
		uint32_t _getCycleOpcode(void);

	public:
		Machine(void);
		virtual ~Machine(void) {};

		bool			step(void);
		virtual void	run(void);

};
#endif
