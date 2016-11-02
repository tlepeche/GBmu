#include "openglwindow.h"
#include "dbwindow.h"

#include <QApplication>

int		main(int argc, char **argv)
{
	QApplication	a(argc, argv);
	OpenGLWindow	w;
	DbWindow		dbgui;

	w.drawPixel(0, 0xFFFF00);
	w.drawPixel(1, 0xFFFF00);
	w.drawPixel(2, 0xFFFF00);

	w.show();
	dbgui.show();
	return (a.exec());
}
