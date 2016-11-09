#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"

#include <thread>
class OpenGLWindow;
class DbWindow;

// QWidget for signals 
#include <QWidget>

// TODO: DEBUG MOD active ! Remove me on release
#define DEBUG

class Gameboy : public Machine, public QWidget
{
	private:
		OpenGLWindow*	_window;
		DbWindow*		_windowDebug;

	public:
		std::thread			*_thread;
		std::atomic<bool>	_debugMode;
		std::atomic<bool>	_willRun;

		std::string			_romPath;

		Gameboy();
		virtual	~Gameboy();

		virtual void	run();
		void			updateGbDb();
		void			reset();
		void			stopThread();
	protected slots:
		void	openRomSlot(std::string path);
		void	gbDbSlot();
		void	stepPressedSlot();
		void	resetPressedSlot();
};

#endif
