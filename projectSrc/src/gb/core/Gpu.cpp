
#include "registerAddr.hpp"
#include "Gpu.hpp"
#include "OpenGLWindow.hpp"
#include "Memory.hpp"
#include "GpuControl.hpp"

Gpu::Gpu(Memory *memory) :
	_clock(0),
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
	t_gpuMode mode = readGpuMode();

	switch (mode)
	{
		case OAM_READ:
			if (_clock >= 80)
			{
				_clock = 0;
				writeGpuMode(VRAM_READ);
			}
			break ;
		case VRAM_READ:
			if (_clock >= 172)
			{
				_clock = 0;
				writeGpuMode(HBLANK);

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
					writeGpuMode(VBLANK);
					_window->renderLater();
					_memory->write_byte(REGISTER_IF, 
							_memory->read_byte(REGISTER_IF) | INTER_VBLANK);
				}
				else
				{
					writeGpuMode(OAM_READ);
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
					writeGpuMode(OAM_READ);
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
	writeGpuMode(OAM_READ);
	_clock = 0;

	// TODO remove when bios is
	std::cout << "set GPU to end, remove me when bios is" << std::endl;
	writeGpuMode(VBLANK);
	_clock = 455;

	_window = OpenGLWindow::Instance();
	_window->initialize();
}

void	Gpu::accClock(unsigned int clock)
{
	_clock += clock;
}

t_gpuMode	Gpu::readGpuMode()
{
	return static_cast<t_gpuMode>(_memory->read_byte(REGISTER_STAT) & 0x3);
}

void		Gpu::writeGpuMode(t_gpuMode mode)
{
	uint8_t	stat = _memory->read_byte(REGISTER_STAT);
	_memory->write_byte(REGISTER_STAT, (stat & 0xFC ) | mode);
}

t_sprite		Gpu::findSprite(uint8_t line, uint8_t x, unsigned int spriteHeight)
{
	t_sprite	*displaySprite = nullptr;
	t_sprite	tmp;

	for (int addr = 0xfe00 ; addr <= 0xfe9f ; addr += 4)
	{
		tmp.y_pos = _memory->read_byte(addr);// - 16;
		tmp.x_pos = _memory->read_byte(addr + 1);// - 8;
		tmp.tile_nbr = _memory->read_byte(addr + 2);
		tmp.options = _memory->read_byte(addr + 3);
		if (tmp.y_pos <= line && line <= (tmp.y_pos + spriteHeight))
		{
			if (tmp.x_pos <= x && x <= (tmp.x_pos + TILE_W))
			{
				if (!displaySprite || displaySprite->x_pos > tmp.x_pos)
					*displaySprite = tmp;
			}
		}
	}
	return *displaySprite;
}

unsigned int	Gpu::findSpritePixel(t_sprite sprite, uint8_t line, uint8_t x, uint8_t spriteHeight)
{
	if (spriteHeight == 16)
		sprite.tile_nbr >>= 1;

	x = sprite.x_flip ? TILE_W - (x - sprite.x_pos) : x - sprite.x_pos;
	line = sprite.y_flip ? spriteHeight + line - sprite.y_pos : line - sprite.y_pos;

	unsigned int tileAddr = (TILES0_ADDR + (sprite.tile_nbr * 16));
	unsigned int start = tileAddr + line;
	uint8_t sdata1 = _memory->read_byte(start * 2);
	uint8_t sdata2 = _memory->read_byte(start * 2 + 1);
	unsigned int rx = TILE_W - x;
	unsigned int colorId = ((sdata1 >> rx) & 1) | (((sdata2 >> rx) & 1) << 1);
	return colorId;
}

unsigned int	Gpu::scanSprite(uint8_t line, uint8_t x, unsigned int pixel)
{
	t_gpuControl	gpuC = (t_gpuControl){_memory->read_byte(REGISTER_LCDC)};
	unsigned int	spritePixel = 0xFFFFFF;

	if (gpuC.sprite)
	{
		uint8_t spriteHeight = gpuC.sprite_size ? 16 : 8;
		t_sprite sprite = findSprite(line, x, spriteHeight);
		unsigned int colorId = findSpritePixel(sprite, line, x, spriteHeight);

		uint8_t	pal = sprite.pal == 0 ? _memory->read_byte(REGISTER_OBP0) :_memory->read_byte(REGISTER_OBP1);
		spritePixel = pal >> (2 * colorId) & 0x03;
	
		if (sprite.bckgrd_prio == 1 || pixel == 0)
			return spritePixel;
	}
	return pixel;

}
