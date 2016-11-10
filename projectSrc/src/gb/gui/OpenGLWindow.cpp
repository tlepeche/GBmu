#include "OpenGLWindow.hpp"

#include <QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QMenuBar>

#include <QFileDialog>

#include <iostream>

// GOOD tuto
// http://doc.qt.io/qt-5/qtwidgets-mainwindows-menus-example.html
#define WIN_WIDTH 160
#define WIN_HEIGHT 144

OpenGLWindow::OpenGLWindow(QWindow *parent)
	: QWindow(parent)
	, m_update_pending(false)
	, m_animating(false)
	, _menuBar(this->genMenuBar())
	, frameBuffer(new QImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_RGB32))
	, m_context(0)
	, m_device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
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

	return menuBar;
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
	delete m_device;
	delete frameBuffer;
}


// Format_RGB32 Format_Mono
/*
void OpenGLWindow::changeFormat(QImage::Format f)
{
	QImage* old = frameBuffer;

	frameBuffer = new QImage(160, 140, f);

	delete old;
}
*/

void OpenGLWindow::drawPixel(uint16_t addr, uint8_t r, uint8_t g, uint8_t b)
{
	drawPixel(addr, ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b << 0));
}

void OpenGLWindow::drawPixel(uint16_t addr, uint32_t color)
{
	((uint32_t*)frameBuffer->bits())[addr] = color;
}

void OpenGLWindow::render(QPainter *painter)
{
	Q_UNUSED(painter);

	painter->drawImage(0, 0, frameBuffer->scaled(size()));
}

void OpenGLWindow::initialize()
{
	unsigned int defaultColor = 0x00000000;

	for (int y = 0 ; y < WIN_HEIGHT; ++y)
		for (int x = 0 ; x < WIN_WIDTH; ++x)
			drawPixel(y * WIN_WIDTH + x, defaultColor);
	renderLater();
}

void OpenGLWindow::render()
{
	if (!m_device)
		m_device = new QOpenGLPaintDevice;


	m_device->setSize(size());
	QPainter painter(m_device);
	render(&painter);
}

void OpenGLWindow::renderLater()
{
	if (!m_update_pending) {
		m_update_pending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool OpenGLWindow::event(QEvent *event)
{
	switch (event->type()) {
		case QEvent::UpdateRequest:
			m_update_pending = false;
			renderNow();
			return true;
		default:
			return QWindow::event(event);
	}
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
	Q_UNUSED(event);

	if (isExposed())
		renderNow();
}

void OpenGLWindow::renderNow()
{
	if (!isExposed())
		return;

	bool needsInitialize = false;

	if (!m_context) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_context->swapBuffers(this);

	if (m_animating)
		renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
	m_animating = animating;

	if (animating)
		renderLater();
}
