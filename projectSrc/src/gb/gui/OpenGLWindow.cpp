#include "OpenGLWindow.hpp"

#include <QCoreApplication>
#include <QPainter>
#include <QMenuBar>
#include <QKeyEvent>
#include <QTimer>
#include <QDragEnterEvent>
#include <QMimeData>

#include <QFileDialog>

#include <iostream>

QTimer timerScreen;

OpenGLWindow::OpenGLWindow(QWidget *parent)
	: QWidget(parent)
	, _menuBar(this->genMenuBar())
	, frameBuffer(new QImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_RGB32))
{
	setAcceptDrops(true);
	resize(500,500);

	connect(&timerScreen, &QTimer::timeout, this, &OpenGLWindow::updateSlot);
	timerScreen.start(16);
}

OpenGLWindow	*OpenGLWindow::Instance()
{
	static OpenGLWindow	*_instance = nullptr;

	if (!_instance)
		_instance = new OpenGLWindow();
	return (_instance);
}

QMenuBar	*OpenGLWindow::genMenuBar()
{
	QMenuBar*	menuBar	= new QMenuBar();
	QMenu*		menu	= new QMenu(tr("Actions"));
	QAction*	openAct	= new QAction(tr("Open"));
	QAction*	gbDbAct = new QAction(tr("Debbuger"));

	connect(openAct, &QAction::triggered, this, &OpenGLWindow::openSlot);
	connect(gbDbAct, &QAction::triggered, this, &OpenGLWindow::gbDbSlot);
	menu->addAction(openAct); openAct->setShortcut(tr("Ctrl+O"));
	menu->addAction(gbDbAct); gbDbAct->setShortcut(tr("Ctrl+D"));
	menuBar->addMenu(menu);

	QMenu*		hard	= new QMenu(tr("Model"));
	QAction*	type0	= new QAction(tr("AUTO"));
	QAction*	type1	= new QAction(tr("GB"));
	QAction*	type2	= new QAction(tr("GBC"));

	connect(type0, &QAction::triggered, this, &OpenGLWindow::gbTypeAUTOSlot);
	connect(type1, &QAction::triggered, this, &OpenGLWindow::gbTypeGBSlot);
	connect(type2, &QAction::triggered, this, &OpenGLWindow::gbTypeGBCSlot);
	hard->addAction(type0);
	hard->addAction(type1);
	hard->addAction(type2);
	menuBar->addMenu(hard);

	return menuBar;
}

void	OpenGLWindow::gbTypeAUTOSlot()
{
	emit gbTypeSign(AUTO);
}

void	OpenGLWindow::gbTypeGBSlot()
{
	emit gbTypeSign(GB);
}

void	OpenGLWindow::gbTypeGBCSlot()
{
	emit gbTypeSign(GBC);
}

void	OpenGLWindow::openSlot()
{
	QString path = QFileDialog::getOpenFileName(NULL, tr("Open XML File 1"), "/home", tr("XML Files (*.xml, *)"));

	emit openRomSign(path.toStdString());
}

void	OpenGLWindow::gbDbSlot()
{
	emit gbDbSign();
}

OpenGLWindow::~OpenGLWindow()
{
	delete _menuBar;
	delete frameBuffer;
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent* e)
{
	emit keyReleaseSign(e->key());
}

void OpenGLWindow::keyPressEvent(QKeyEvent* e)
{
	emit keyPressSign(e->key());
}

void OpenGLWindow::drawPixel(uint16_t addr, uint8_t r, uint8_t g, uint8_t b)
{
	drawPixel(addr, ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b << 0));
}

void OpenGLWindow::drawPixel(uint16_t addr, uint32_t color)
{
	((uint32_t*)frameBuffer->bits())[addr] = color;
}

void OpenGLWindow::initialize()
{
	unsigned int defaultColor = 0x00000000;

	for (int y = 0 ; y < WIN_HEIGHT; ++y)
		for (int x = 0 ; x < WIN_WIDTH; ++x)
			drawPixel(y * WIN_WIDTH + x, defaultColor);
}

void OpenGLWindow::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void OpenGLWindow::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	emit openRomSign(mimeData->urls().at(0).toLocalFile().toStdString());
}

void OpenGLWindow::paintEvent(__attribute__((unused)) QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawImage(0, 0, frameBuffer->scaled(size()));
	painter.end();
}

void OpenGLWindow::updateSlot(void)
{
	update();
}
