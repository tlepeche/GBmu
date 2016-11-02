#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <QDialog>

namespace Ui {
	class DbWindow;
}

class QTableWidget;

class DbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DbWindow(QWidget *parent = 0);
    ~DbWindow();

	QTableWidget *tableRegisters;
	QTableWidget *tableOtherRegisters;
	QTableWidget *tableVideoRegisters;
	QTableWidget *tableDisassembler;
	QTableWidget *tableMemory;
private:
    Ui::DbWindow *ui;
};

#endif // DBWINDOW_H
