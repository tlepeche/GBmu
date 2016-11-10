
#include "Gpu.hpp"
#include "OpenGLWindow.hpp"

Gpu		Gpu::_gpuInstance = Gpu();

Gpu		&Gpu::Instance()
{
	return Gpu::_gpuInstance;
}

Gpu::Gpu() :
	_clock(0),
	_mode(OAM_READ),
	_line(0),
	_window(nullptr)
{
}

Gpu::~Gpu()
{

}

#include <iostream>
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

				_window->drawPixel(_line * 2, 0x00FFFFFF);
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
