#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"
#include "OpenGLWindow.hpp"

enum htype {
	AUTO,
	GB,
	GBC
};

// QWidget for signals 
#include <QWidget>

class Gameboy : public Machine, public QWidget
{
	private:
		OpenGLWindow*	_window;

	public:
		bool	is_debug_mode;

		Gameboy();
		virtual	~Gameboy();

		virtual void	run();

	protected slots:
		void	openRomSlot(std::string path);
};

#endif
