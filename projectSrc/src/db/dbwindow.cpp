#include "dbwindow.h"
#include "ui_dbwindow.h"

DbWindow::DbWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbWindow)
{
    ui->setupUi(this);

	tableRegisters		= this->findChild<QTableWidget*>("tableRegisters");
	tableOtherRegisters	= this->findChild<QTableWidget*>("tableOtherRegisters");
	tableVideoRegisters	= this->findChild<QTableWidget*>("tableVideoRegisters");
	tableDisassembler	= this->findChild<QTableWidget*>("tableDisassembler");
	tableMemory			= this->findChild<QTableWidget*>("tableMemory");
}

DbWindow::~DbWindow()
{
    delete ui;
}
