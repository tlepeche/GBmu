
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

class Gpu
{
	protected:
		unsigned int	_clock;
		t_gpuMode		_mode;
		unsigned int	_line;
		OpenGLWindow	*_window;
		Memory			&_memory;

		unsigned int	_activeTile; // TODO: check 
	private:
		Gpu();
		~Gpu();

	public:
		static Gpu		_gpuInstance;
		static Gpu		&Instance();

		void	step();
		void	init();
		void	accClock(unsigned int clock);
		void	scanActLine();
		unsigned int	scanPixel(unsigned int x);
};
