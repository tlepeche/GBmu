
#pragma once

typedef	enum	s_gpuMode
{
	OAM_READ,
	VRAM_READ,
	HBLANK,
	VBLANK
}				t_gpuMode;

class OpenGLWindow;
#include <stdint.h>
#include "Memory.hpp"

class Machine;

class Gpu
{
	friend Machine;
	protected:
		unsigned int	_clock;
		t_gpuMode		_mode;
		OpenGLWindow	*_window;
		Memory			*_memory;

	private:
		Gpu(Memory *memory);
		~Gpu();

	public:
		void	step();
		void	init();
		void	accClock(unsigned int clock);
		void	scanActLine();
		unsigned int	scanPixel(uint8_t line, unsigned int x);

		std::string		toString();
};
