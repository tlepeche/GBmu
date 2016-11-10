
#include "Gpu.hpp"
#include "OpenGLWindow.hpp"
#include "Memory.hpp"

Gpu		Gpu::_gpuInstance = Gpu();

Gpu		&Gpu::Instance()
{
	return Gpu::_gpuInstance;
}

Gpu::Gpu() :
	_clock(0),
	_mode(OAM_READ),
	_line(0),
	_window(nullptr),
	_memory(Memory::Instance())
{
	_activeTile = 0; // TODO: CHECK how to get it
}

Gpu::~Gpu()
{

}

#define TILES0_ADDR 0x8000 // Go to 0x8FFF /    0->255
#define TILES1_ADDR 0x8800 // Go to 0x97FF / -128->127

#define TILE_W 8 // bits
#define TILE_PIXEL_SIZE 2 // bits on Gb
#define BYTE_SIZE 8 // byte size 0000 0000 ;)
#define PIXEL_BY_BYTE (BYTE_SIZE / TILE_PIXEL_SIZE)
#define TILE_LINE_SIZE (TILE_W / PIXEL_BY_BYTE)

#define MAP_W 32 
#define MAP0_ADDR 0x9800 // 32*32 tile
#define MAP1_ADDR 0x9C00 // 32*32 tile

#include <iostream>
unsigned int	Gpu::scanPixel(unsigned int x)
{
	unsigned int pixel = 0xFFFFFF;
	/*
	unsigned int tileMapAddr = _activeTile ? MAP1_ADDR : MAP0_ADDR;
	unsigned int tileSetAddr = _activeTile ? TILES1_ADDR : TILES0_ADDR;
	std::cout << std::hex << (tileMapAddr + _line * MAP_W + x) << std::endl;
	unsigned int tileId = _memory.read_byte(tileMapAddr + _line * MAP_W + x); // TODO: use scroll X / Y here
	unsigned int tileAddr = tileSetAddr + tileId;

	unsigned int sy = _line % TILE_W;
	unsigned int sx = x % TILE_W;

	uint8_t	sdata = 0; //_memory.read_byte(tileAddr + (sy * TILE_LINE_SIZE) + (x / PIXEL_BY_BYTE));
	pixel = sdata >> (2 * (x % PIXEL_BY_BYTE));
	*/
	return pixel;
}

void	Gpu::scanActLine()
{
	uint16_t		addrLine;
	unsigned int	pixel;

	for (int x = 0 ; x < WIN_WIDTH ; ++x) {
		addrLine = _line * WIN_WIDTH + x;
		pixel = scanPixel(x);
		_window->drawPixel(addrLine, pixel);
	}
}

void	Gpu::step()
{
	switch (_mode)
	{
		case OAM_READ:
			if (_clock >= 80)
			{
				std::cout << "OAM_READ" << std::endl;
				_clock = 0;
				_mode = VRAM_READ;
			}
			break ;
		case VRAM_READ:
			if (_clock >= 172)
			{
				std::cout << "VRAM_READ" << std::endl;
				_clock = 0;
				_mode = HBLANK;

				scanActLine();
			}
			break ;
		case HBLANK:
			if (_clock >= 204)
			{
				_clock = 0;
				++_line;

				std::cout << "HBLANK" << std::endl;
				if (_line == 143)
				{
					_mode = VBLANK;
					_window->renderLater();
				}
				else
				{
					_mode = OAM_READ;
				}
			}
			break ;
		case VBLANK:
				if (_clock >= 456)
				{
					_clock = 0;
					++_line;

				std::cout << "VBLANK" << std::endl;
					if (_line > 153)
					{
						_mode = OAM_READ;
						_line = 0;
					}
				}
			break ;
		default:
			break ;
	}
}

void	Gpu::init()
{
	_mode = OAM_READ;
	_clock = 0;
	_line = 0;
	_window = OpenGLWindow::Instance();
	_window->initialize();
}

void	Gpu::accClock(unsigned int clock)
{
	_clock += clock;
}
