
#pragma once

#include <stdint.h>

typedef	enum	s_gpuMode
{
	OAM_READ = 2,
	VRAM_READ = 3,
	HBLANK = 0,
	VBLANK = 1
}				t_gpuMode;

typedef struct	s_gpuStat
{
	union {
		struct {
			uint8_t unused_bit:1;
			uint8_t	interupt_coincid:1;
			uint8_t	interupt_oam:1;
			uint8_t	interupt_vblank:1;
			uint8_t	interupt_hblank:1;
			uint8_t	coincidence:1;
			uint8_t mode:2;
		};
		uint8_t stat;
	};
}				t_gpuStat;

class OpenGLWindow;
#include "Memory.hpp"
#include "Sprites.hpp"

class Machine;

class Gpu
{
	friend Machine;
	protected:
		unsigned int	_clock;
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

		t_gpuMode	readGpuMode(void);
		void		writeGpuMode(t_gpuMode mode);

		unsigned int	scanPixel(uint8_t line, unsigned int x);
		unsigned int	scanSprite(uint8_t line, uint8_t x, unsigned int pixel);
		unsigned int	findSpritePixel(t_sprite sprite, uint8_t line, uint8_t x, uint8_t spriteHeight);
		t_sprite		findSprite(uint8_t line, uint8_t x, unsigned int spriteHeight);

		std::string		toString();
};
