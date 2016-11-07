
#include <QApplication>
#include "Gameboy.hpp"

int		main(int argc, char **argv)
{
	QApplication	a(argc, argv);
	Gameboy			gb;

	return (a.exec());
}
