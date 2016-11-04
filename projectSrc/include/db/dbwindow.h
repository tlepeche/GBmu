#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <QDialog>

namespace Ui {
	class DbWindow;
}

class QTableWidget;
class Memory;
typedef struct s_register t_register;

class DbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DbWindow(QWidget *parent = 0);
    ~DbWindow();

	void 	updateRegister(t_register& r);
	void 	updateMemory(Memory& m);

	QTableWidget *tableRegisters;
	QTableWidget *tableOtherRegisters;
	QTableWidget *tableVideoRegisters;
	QTableWidget *tableDisassembler;
	QTableWidget *tableMemory;
private:
    Ui::DbWindow *ui;
};

#endif // DBWINDOW_H
