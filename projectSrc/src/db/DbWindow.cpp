#include "DbWindow.hpp"
#include "ui_dbwindow.h"

#include "register.hpp"
#include "Memory.hpp"

#include <QTimer>
QTimer timer;

DbWindow::DbWindow(t_register* r, Memory* mem) :
    QDialog(nullptr),
    ui(new Ui::DbWindow),
	_r(r),
	_mem(mem),
	_start(0xD000)
{
    ui->setupUi(this);

	tableRegisters		= this->findChild<QTableWidget*>("tableRegisters");
	tableOtherRegisters	= this->findChild<QTableWidget*>("tableOtherRegisters");
	tableVideoRegisters	= this->findChild<QTableWidget*>("tableVideoRegisters");
	tableDisassembler	= this->findChild<QTableWidget*>("tableDisassembler");
	tableMemory			= this->findChild<QTableWidget*>("tableMemory");

	buttonReset			= this->findChild<QPushButton*>("buttonReset");
	buttonStep			= this->findChild<QPushButton*>("buttonStep");
	buttonFrame			= this->findChild<QPushButton*>("buttonFrame");

	lineAddr			= this->findChild<QLineEdit*>("lineAddr");
	tableMemory->resizeColumnsToContents();

	connect(buttonStep, &QPushButton::pressed, this, &DbWindow::stepPressedSlot);
	connect(lineAddr, &QLineEdit::editingFinished, this, &DbWindow::lineAddrEditedSlot);

	connect(&timer, &QTimer::timeout, this, &DbWindow::updateAllSlot);
	timer.start(100);
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
	customSetItem(tableRegisters, 0, 0, "%0.4X", r.PC);
	customSetItem(tableRegisters, 0, 1, "%0.4X", r.AF);
	customSetItem(tableRegisters, 0, 2, "%0.4X", r.BC);
	customSetItem(tableRegisters, 0, 3, "%0.4X", r.DE);
	customSetItem(tableRegisters, 0, 4, "%0.4X", r.HL);
	customSetItem(tableRegisters, 0, 5, "%0.4X", r.SP);
}

void DbWindow::updateDisassembler(t_register& r, Memory& mem)
{
	int		i;
	int		opcode;

	for (i = 0; i < tableDisassembler->rowCount(); ++i)
	{
		opcode = mem.read_byte(r.PC + i);
		customSetItem(tableDisassembler, i, 0, "%0.2X", opcode);
	}
}

void DbWindow::updateMemory(Memory& m)
{
	unsigned int row, col, curr;

	for (row = 0 ; row < 9 ; ++row) {
		curr = _start + row * 0x10;
		customSetItem(tableMemory, row - 1, col + 1, "%0.4X:", curr);
		for (col = 0 ; col <= 0xF ; ++col)
			customSetItem(tableMemory, row, col + 1, "%0.2X", m.read_byte(curr + col));
	}
}

#include <sstream>

void	DbWindow::lineAddrEditedSlot()
{
	std::stringstream	ss;
	unsigned int		max = 0xFFFF - 8 * 0x10;
	ss << std::hex << lineAddr->text().toStdString();
	ss >> _start;

	if (_start >= max)
		_start = max;
}

void	DbWindow::stepPressedSlot()
{
	emit	stepPressedSign();
}

// This function il call every 100ms see _timer 
void	DbWindow::updateAllSlot()
{
	updateRegister(*_r);
	updateMemory(*_mem);
	updateDisassembler(*_r, *_mem);
}

DbWindow::~DbWindow()
{
    delete ui;
}
