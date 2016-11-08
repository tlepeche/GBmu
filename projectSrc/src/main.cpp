
#include <QApplication>
#include "Gameboy.hpp"

int		main(int argc __attribute__ (( unused )), char **argv __attribute__ (( unused )))
{
	QApplication	a(argc, argv);
	Gameboy			gb;

	return (a.exec());
}
