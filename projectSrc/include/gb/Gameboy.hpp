#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"
#include "OpenGLWindow.hpp"

enum htype {
	AUTO,
	GB,
	GBC
};

class Gameboy : public Machine
{
	private:
		OpenGLWindow*	_window;

	public:
		bool	is_debug_mode;

		Gameboy();
		virtual	~Gameboy();

		virtual void	run();
};

#endif
