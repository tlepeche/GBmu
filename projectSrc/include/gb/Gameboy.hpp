#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "Machine.hpp"

#include <atomic>
#include <list>
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
		std::atomic<bool>	_stepMode;
		std::atomic<bool>	_willRun;
		std::list<uint16_t>	_breakpoints;

		std::string			_romPath;

		Gameboy();
		virtual	~Gameboy();

		void			gstep();
		virtual void	run();
		void			updateGbDb();
		void			reset();
		void			stopThread();

		bool			isBreakpoint(uint16_t addr);
	protected slots:
		void	openRomSlot(std::string path);
		void	gbDbSlot();
		void	stepPressedSlot();
		void	framePressedSlot();
		void	switchStepModeSlot();
		void	addBreakpointSlot(uint16_t addr);
		void	delBreakpointSlot(uint16_t addr);
		void	resetPressedSlot();
};

#endif
