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

OpenGLWindow::OpenGLWindow(QWindow *parent)
	: QWindow(parent)
	, m_update_pending(false)
	, m_animating(false)
	, frameBuffer(new QImage(160, 140, QImage::Format_RGB32))
	, _menuBar(this->genMenuBar())
	, m_context(0)
	, m_device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

QMenuBar	*OpenGLWindow::genMenuBar()
{
	QMenuBar*	menuBar	= new QMenuBar();
	QMenu*		menu	= new QMenu(tr("Files"));
	QAction*	openAct	= new QAction(tr("Open"));

	connect(openAct, &QAction::triggered, this, &OpenGLWindow::openSlot);
	menu->addAction(openAct);
	menuBar->addMenu(menu);

	return menuBar;
}

void	OpenGLWindow::openSlot()
{
	QString path = QFileDialog::getOpenFileName(NULL, tr("Open XML File 1"), "/home", tr("XML Files (*.xml, *)"));

	emit openRomSign(path.toStdString());
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
}

void OpenGLWindow::render()
{
	if (!m_device)
		m_device = new QOpenGLPaintDevice;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
