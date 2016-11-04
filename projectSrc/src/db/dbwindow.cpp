#include "dbwindow.h"
#include "ui_dbwindow.h"

#include "register.h"
#include "Memory.hpp"

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
	tableMemory->resizeColumnsToContents();
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

void DbWindow::updateMemory(Memory& m)
{
	unsigned int _start = 0xD000;
	unsigned int row, col, curr;

	for (row = 0 ; row < 9 ; ++row) {
		curr = _start + row * 0x10;
		customSetItem(tableMemory, row - 1, col + 1, "%0.4X:", curr);
		for (col = 0 ; col <= 0xF ; ++col)
			customSetItem(tableMemory, row, col + 1, "%0.2X", m.read_byte(curr + col));
	}
}

DbWindow::~DbWindow()
{
    delete ui;
}
