#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"

enum htype {
	AUTO,
	GB,
	GBC
};

#include <thread>
class OpenGLWindow;
class DbWindow;

// QWidget for signals 
#include <QWidget>

class Gameboy : public Machine, public QWidget
{
	private:
		OpenGLWindow*	_window;
		DbWindow*		_windowDebug;

	public:
		std::thread			*_thread;
		std::atomic<bool>	_debugMode;
		std::atomic<bool>	_willRun;

		Gameboy();
		virtual	~Gameboy();

		virtual void	run();
		void			updateGbDb();

	protected slots:
		void	openRomSlot(std::string path);
		void	gbDbSlot();
		void	stepPressedSlot();
};

#endif
