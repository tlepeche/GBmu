#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QMenuBar;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
	public:
		explicit OpenGLWindow(QWindow *parent = 0);
		~OpenGLWindow();

		virtual void render(QPainter *painter);
		virtual void render();

		virtual void initialize();

		void setAnimating(bool animating);
		//void changeFormat(QImage::Format f);
		void drawPixel(uint16_t addr, uint8_t r, uint8_t g, uint8_t b);
		void drawPixel(uint16_t addr, uint32_t color);

		QMenuBar*	genMenuBar(void);
	public slots:
		void renderLater();
		void renderNow();

	private slots:
		void openSlot();
		void gbDbSlot();

	signals:
		void openRomSign(std::string path);
		void gbDbSign();

	protected:
		bool event(QEvent *event) Q_DECL_OVERRIDE;

		void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

	private:
		bool m_update_pending;
		bool m_animating;

		QMenuBar			*_menuBar;
		QImage				*frameBuffer;
		QOpenGLContext 		*m_context;
		QOpenGLPaintDevice	*m_device;
};

#endif // OPENGLWINDOW_H
