
#include "registerAddr.hpp"
#include "Gpu.hpp"
#include "OpenGLWindow.hpp"
#include "Memory.hpp"
#include "GpuControl.hpp"

Gpu::Gpu(Memory *memory) :
	_clock(0),
	_mode(OAM_READ),
	_window(nullptr),
	_memory(memory)
{
}

Gpu::~Gpu()
{

}

#define TILES0_ADDR 0x8000 // Go to 0x8FFF /    0->255
#define TILES1_ADDR 0x8800 // Go to 0x97FF / -128->127

#define TILE_W 8 // bits
#define TILE_H 8
#define BYTE_SIZE 8 // byte size 0000 0000 ;)

#define MAP_W 32 
#define MAP0_ADDR 0x9800 // 32*32 tile
#define MAP1_ADDR 0x9C00 // 32*32 tile

#include <iostream>

std::string	Gpu::toString()
{
	t_gpuControl	gpuC = (t_gpuControl){_memory->read_byte(REGISTER_LCDC)};
	char			buf[32];

	std::string			s;
	
	sprintf(buf, "[%d, %d, %d, %d, %d, %d, %d, %d]",
		gpuC.background ,
		gpuC.sprite ,
		gpuC.sprite_size ,
		gpuC.tile_map ,
		gpuC.tile_set ,
		gpuC.window ,
		gpuC.wtile_map ,
		gpuC.display
			);
	s = std::string(buf);
	return (s);
}

unsigned int gbColors[4] = {0x00FFFFFF, 0x00C0C0C0, 0x00606060, 0x00000000};

unsigned int	Gpu::scanPixel(uint8_t line, unsigned int x)
{
	t_gpuControl	gpuC = (t_gpuControl){_memory->read_byte(REGISTER_LCDC)};
	uint8_t			scy = _memory->read_byte(REGISTER_SCY);
	uint8_t			scx = _memory->read_byte(REGISTER_SCX);

	unsigned int tileMapAddr = gpuC.tile_map ? MAP1_ADDR : MAP0_ADDR;
	unsigned int tileSetAddr = gpuC.tile_set ? TILES1_ADDR : TILES0_ADDR;
	unsigned int tileId = _memory->read_byte(
			tileMapAddr
			+ (((line / TILE_W) + scy) * MAP_W)
			+ (x / TILE_W) + scx); // TODO: use scroll X / Y here
	unsigned int tileAddr = tileSetAddr + tileId * TILE_H * 2;

	unsigned int sy = line % TILE_W;
	unsigned int sx = x % TILE_W;
	unsigned int rsx = BYTE_SIZE - sx - 1;

	uint8_t	sdata1 = _memory->read_byte(tileAddr + (sy * 2));
	uint8_t	sdata2 = _memory->read_byte(tileAddr + (sy * 2) + 1);
	unsigned int colorId = ((sdata1 >> rsx) & 1) | (((sdata2 >> rsx) & 1) << 1);

	return gbColors[colorId & 3];
}

void	Gpu::scanActLine()
{
	uint16_t		addrLine;
	unsigned int	pixel;
	uint8_t			line = _memory->read_byte(REGISTER_LY);

	for (int x = 0 ; x < WIN_WIDTH ; ++x) {
		addrLine = line * WIN_WIDTH + x;
		pixel = scanPixel(line, x);
		_window->drawPixel(addrLine, pixel);
	}
}

#include "interrupt.hpp"

void	Gpu::step()
{
	uint8_t	line = _memory->read_byte(REGISTER_LY);

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
				_memory->write_byte(REGISTER_LY, ++line);

				if (line == 143)
				{
					_mode = VBLANK;
					_window->renderLater();
					_memory->write_byte(REGISTER_IF, 
							_memory->read_byte(REGISTER_IF) | INTER_VBLANK);
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
					_memory->write_byte(REGISTER_LY, ++line);

					if (line > 153)
					{
						_mode = OAM_READ;
						_memory->write_byte(REGISTER_LY, 0);
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
