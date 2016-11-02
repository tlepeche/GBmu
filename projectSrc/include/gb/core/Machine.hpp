#ifndef _MACHINE_HPP
# define _MACHINE_HPP

class Machine
{
	private:
		Timer::Timer &_clock;
		Cpu::Cpu &_cpu;
		Rom::Rom &_rom;
		Gpu::Gpu &_gpu;

		_getFrequencyFrameTimeGpu(void);
		_getCycleOpcode(void);

	public:
		Machine(void) : _clock(Timer::Instance()),
						_cpu(Cpu::Instance());
		~Machine(void);
		void run(void);

}
#endif
