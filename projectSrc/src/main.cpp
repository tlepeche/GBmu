
#include <QApplication>
#include "Gameboy.hpp"

int		main(int argc, char *argv[])
{
	QApplication	a(argc, argv);

	if (argc > 1)
	{
		Gameboy			gb(argv[1]);
		return (a.exec());
	}
	else
	{
		Gameboy			gb;
		return (a.exec());
	}
}
