
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
#include "Sprites.hpp"

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
		unsigned int	scanSprite(uint8_t line, uint8_t x, unsigned int pixel);
		unsigned int	findSpritePixel(t_sprite sprite, uint8_t line, uint8_t x, uint8_t spriteHeight);
		t_sprite		findSprite(uint8_t line, uint8_t x, unsigned int spriteHeight);

		std::string		toString();
};
