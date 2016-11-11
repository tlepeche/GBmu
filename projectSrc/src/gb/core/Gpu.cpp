
#include "registerAddr.hpp"
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
unsigned int	Gpu::scanPixel(uint8_t line, unsigned int x)
{
	unsigned int pixel = 0xFFFFFF;
	unsigned int tileMapAddr = _activeTile ? MAP1_ADDR : MAP0_ADDR;
	unsigned int tileSetAddr = _activeTile ? TILES1_ADDR : TILES0_ADDR;
	unsigned int tileId = _memory.read_byte(tileMapAddr + (line / TILE_W * MAP_W) + (x / TILE_W)); // TODO: use scroll X / Y here
	unsigned int tileAddr = tileSetAddr + tileId;

	
	unsigned int sy = line % TILE_W;
	unsigned int sx = x % TILE_W;

	uint8_t	sdata = _memory.read_byte(tileAddr + (sy * TILE_LINE_SIZE) + (sx / PIXEL_BY_BYTE));
	unsigned int colorId = (sdata >> (2 * (x % PIXEL_BY_BYTE))) & 0x3;

	switch (colorId)
	{
		case 0:
			pixel = 0x00FFFFFF;
			break ;
		case 1:
			pixel = 0x00C0C0C0;
			break ;
		case 2:
			pixel = 0x00606060;
			break ;
		case 3:
			pixel = 0x00000000;
			break;
		default:
			pixel = 0x00FF0000; // TODO: impossible case !
			break ;
	}
	return pixel;
}

void	Gpu::scanActLine()
{
	uint16_t		addrLine;
	unsigned int	pixel;
	uint8_t			line = _memory.read_byte(REGISTER_LY);

	for (int x = 0 ; x < WIN_WIDTH ; ++x) {
		addrLine = line * WIN_WIDTH + x;
		pixel = scanPixel(line, x);
		_window->drawPixel(addrLine, pixel);
	}
}

void	Gpu::step()
{
	uint8_t	line = _memory.read_byte(REGISTER_LY);

	switch (_mode)
	{
		case OAM_READ:
			if (_clock >= 80)
			{
				_clock = 0;
				_mode = VRAM_READ;
			}
			break ;
		case VRAM_READ:
			if (_clock >= 172)
			{
				_clock = 0;
				_mode = HBLANK;

				scanActLine();
			}
			break ;
		case HBLANK:
			if (_clock >= 204)
			{
				_clock = 0;
				_memory.write_byte(REGISTER_LY, ++line);

				if (line == 143)
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
					_memory.write_byte(REGISTER_LY, ++line);

					if (line > 153)
					{
						_mode = OAM_READ;
						_memory.write_byte(REGISTER_LY, 0);
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
	_window = OpenGLWindow::Instance();
	_window->initialize();
}

void	Gpu::accClock(unsigned int clock)
{
	_clock += clock;
}
