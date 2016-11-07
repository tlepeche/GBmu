#include "OpenGLWindow.hpp"
#include "DbWindow.hpp"

#include <QApplication>


#include "Memory.hpp"
#include "register.hpp"

#include "Machine.hpp"

int		main(int argc, char **argv)
{
	Machine 	gameboy;

	gameboy.run();
	/*
	QApplication	a(argc, argv);
	OpenGLWindow	w;
	DbWindow		dbgui;
	Memory&			mem = Memory::Instance();

	w.drawPixel(0, 0xFFFF00);
	w.drawPixel(1, 0xFFFF00);
	w.drawPixel(2, 0xFFFF00);

	t_register	r;

	int lol = 0;
	for (lol = 0; lol < 60000; ++lol)
		mem.write_byte(lol, lol);
	dbgui.updateRegister(r);
	dbgui.updateMemory(mem);

	w.show();
	dbgui.show();
	return (a.exec());
	*/
}
