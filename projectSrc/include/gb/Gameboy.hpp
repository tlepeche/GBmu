#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"
#include "OpenGLWindow.hpp"

enum htype {
	AUTO,
	GB,
	GBC
};

#include <thread>

// QWidget for signals 
#include <QWidget>

class Gameboy : public Machine, public QWidget
{
	private:
		OpenGLWindow*	_window;

	public:
		bool				is_debug_mode;
		std::thread			*_thread;
		std::atomic<bool>	_willRun;

		Gameboy();
		virtual	~Gameboy();

		virtual void	run();

	protected slots:
		void	openRomSlot(std::string path);
};

#endif
