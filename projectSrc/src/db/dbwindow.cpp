#include "dbwindow.h"
#include "ui_dbwindow.h"

#include "register.h"

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
static inline
void customSetItem(QTableWidget* table, int x, int y, const char *format, int value)
{
	char	buffer[32];

	sprintf(buffer, format, value);
	table->setItem(x, y, new QTableWidgetItem(buffer));
}

void DbWindow::updateRegister(t_register& r)
{
// TODO: after merge 
//	customSetItem(tableRegisters, 0, 0, "%0.4X", r.PC);
	customSetItem(tableRegisters, 0, 1, "%0.4X", r.AF);
	customSetItem(tableRegisters, 0, 2, "%0.4X", r.BC);
	customSetItem(tableRegisters, 0, 3, "%0.4X", r.DE);
	customSetItem(tableRegisters, 0, 4, "%0.4X", r.HL);
//	customSetItem(tableRegisters, 0, 5, "%0.4X", r.SP);
}

DbWindow::~DbWindow()
{
    delete ui;
}
