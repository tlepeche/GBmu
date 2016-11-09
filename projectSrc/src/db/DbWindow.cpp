#include "DbWindow.hpp"
#include "ui_dbwindow.h"

#include "register.hpp"
#include "Memory.hpp"
#include "registerAddr.hpp"

#include <QTimer>
QTimer timer;

#include <QFileDialog>
#include <QInputDialog>

static inline
void customSetItem(QTableWidget* table, int x, int y, const char *format, int value)
{
	char	buffer[32];

	sprintf(buffer, format, value);
	table->setItem(x, y, new QTableWidgetItem(buffer));
}

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
	buttonOpen			= this->findChild<QPushButton*>("buttonOpen");
	buttonBpAdd			= this->findChild<QPushButton*>("buttonBpAdd");

	lineAddr			= this->findChild<QLineEdit*>("lineAddr");
	tableMemory->resizeColumnsToContents();

	connect(buttonStep, &QPushButton::pressed, this, &DbWindow::stepPressedSlot);
	connect(buttonReset, &QPushButton::pressed, this, &DbWindow::resetPressedSlot);
	connect(buttonOpen, &QPushButton::pressed, this, &DbWindow::openPressedSlot);
	connect(buttonBpAdd, &QPushButton::pressed, this, &DbWindow::bpAddPressedSlot);
	connect(lineAddr, &QLineEdit::editingFinished, this, &DbWindow::lineAddrEditedSlot);

	connect(&timer, &QTimer::timeout, this, &DbWindow::updateAllSlot);
	timer.start(100);


	customSetItem(tableOtherRegisters, 0, 0, "%.2X", REGISTER_P1);
	customSetItem(tableOtherRegisters, 1, 0, "%.2X", REGISTER_SB);
	customSetItem(tableOtherRegisters, 2, 0, "%.2X", REGISTER_SC);
	customSetItem(tableOtherRegisters, 3, 0, "%.2X", REGISTER_DIV);
	customSetItem(tableOtherRegisters, 4, 0, "%.2X", REGISTER_TIMA);
	customSetItem(tableOtherRegisters, 5, 0, "%.2X", REGISTER_TMA);
	customSetItem(tableOtherRegisters, 6, 0, "%.2X", REGISTER_TAC);
	customSetItem(tableOtherRegisters, 7, 0, "%.2X", REGISTER_KEY1);
	customSetItem(tableOtherRegisters, 8, 0, "%.2X", REGISTER_VBK);
	customSetItem(tableOtherRegisters, 9, 0, "%.2X", REGISTER_HDMA1);
	customSetItem(tableOtherRegisters, 10, 0, "%.2X", REGISTER_HDMA2);
	customSetItem(tableOtherRegisters, 11, 0, "%.2X", REGISTER_HDMA3);
	customSetItem(tableOtherRegisters, 12, 0, "%.2X", REGISTER_HDMA4);
	customSetItem(tableOtherRegisters, 13, 0, "%.2X", REGISTER_HDMA5);
	customSetItem(tableOtherRegisters, 14, 0, "%.2X", REGISTER_SVBK);
	customSetItem(tableOtherRegisters, 15, 0, "%.2X", REGISTER_IF);
	customSetItem(tableOtherRegisters, 16, 0, "%.2X", REGISTER_IE);

	customSetItem(tableVideoRegisters, 0, 0, "%.2X", REGISTER_LCDC);
	customSetItem(tableVideoRegisters, 1, 0, "%.2X", REGISTER_STAT);
	customSetItem(tableVideoRegisters, 2, 0, "%.2X", REGISTER_SCY);
	customSetItem(tableVideoRegisters, 3, 0, "%.2X", REGISTER_SCX);
	customSetItem(tableVideoRegisters, 4, 0, "%.2X", REGISTER_LY);
	customSetItem(tableVideoRegisters, 5, 0, "%.2X", REGISTER_LYC);
	customSetItem(tableVideoRegisters, 6, 0, "%.2X", REGISTER_DMA);
	customSetItem(tableVideoRegisters, 7, 0, "%.2X", REGISTER_BGP);
	customSetItem(tableVideoRegisters, 8, 0, "%.2X", REGISTER_OBP0);
	customSetItem(tableVideoRegisters, 9, 0, "%.2X", REGISTER_OBP1);
	customSetItem(tableVideoRegisters, 10, 0, "%.2X", REGISTER_WY);
	customSetItem(tableVideoRegisters, 11, 0, "%.2X", REGISTER_WX);
	customSetItem(tableVideoRegisters, 12, 0, "%.2X", REGISTER_BCPS);
	customSetItem(tableVideoRegisters, 13, 0, "%.2X", REGISTER_BCPD);
	customSetItem(tableVideoRegisters, 14, 0, "%.2X", REGISTER_OCPS);
	customSetItem(tableVideoRegisters, 15, 0, "%.2X", REGISTER_OCPD);

}

void DbWindow::updateRegister(t_register& r)
{
	customSetItem(tableRegisters, 0, 0, "%.4X", r.PC);
	customSetItem(tableRegisters, 0, 1, "%.4X", r.AF);
	customSetItem(tableRegisters, 0, 2, "%.4X", r.BC);
	customSetItem(tableRegisters, 0, 3, "%.4X", r.DE);
	customSetItem(tableRegisters, 0, 4, "%.4X", r.HL);
	customSetItem(tableRegisters, 0, 5, "%.4X", r.SP);
}

void DbWindow::updateOtherRegister(Memory& mem)
{
	customSetItem(tableOtherRegisters, 0, 1, "%.2X", mem.read_byte(REGISTER_P1));
	customSetItem(tableOtherRegisters, 1, 1, "%.2X", mem.read_byte(REGISTER_SB));
	customSetItem(tableOtherRegisters, 2, 1, "%.2X", mem.read_byte(REGISTER_SC));
	customSetItem(tableOtherRegisters, 3, 1, "%.2X", mem.read_byte(REGISTER_DIV));
	customSetItem(tableOtherRegisters, 4, 1, "%.2X", mem.read_byte(REGISTER_TIMA));
	customSetItem(tableOtherRegisters, 5, 1, "%.2X", mem.read_byte(REGISTER_TMA));
	customSetItem(tableOtherRegisters, 6, 1, "%.2X", mem.read_byte(REGISTER_TAC));
	customSetItem(tableOtherRegisters, 7, 1, "%.2X", mem.read_byte(REGISTER_KEY1));
	customSetItem(tableOtherRegisters, 8, 1, "%.2X", mem.read_byte(REGISTER_VBK));
	customSetItem(tableOtherRegisters, 9, 1, "%.2X", mem.read_byte(REGISTER_HDMA1));
	customSetItem(tableOtherRegisters, 10, 1, "%.2X", mem.read_byte(REGISTER_HDMA2));
	customSetItem(tableOtherRegisters, 11, 1, "%.2X", mem.read_byte(REGISTER_HDMA3));
	customSetItem(tableOtherRegisters, 12, 1, "%.2X", mem.read_byte(REGISTER_HDMA4));
	customSetItem(tableOtherRegisters, 13, 1, "%.2X", mem.read_byte(REGISTER_HDMA5));
	customSetItem(tableOtherRegisters, 14, 1, "%.2X", mem.read_byte(REGISTER_SVBK));
	customSetItem(tableOtherRegisters, 15, 1, "%.2X", mem.read_byte(REGISTER_IF));
	customSetItem(tableOtherRegisters, 16, 1, "%.2X", mem.read_byte(REGISTER_IE));

	customSetItem(tableVideoRegisters, 0, 1, "%.2X", mem.read_byte(REGISTER_LCDC));
	customSetItem(tableVideoRegisters, 1, 1, "%.2X", mem.read_byte(REGISTER_STAT));
	customSetItem(tableVideoRegisters, 2, 1, "%.2X", mem.read_byte(REGISTER_SCY));
	customSetItem(tableVideoRegisters, 3, 1, "%.2X", mem.read_byte(REGISTER_SCX));
	customSetItem(tableVideoRegisters, 4, 1, "%.2X", mem.read_byte(REGISTER_LY));
	customSetItem(tableVideoRegisters, 5, 1, "%.2X", mem.read_byte(REGISTER_LYC));
	customSetItem(tableVideoRegisters, 6, 1, "%.2X", mem.read_byte(REGISTER_DMA));
	customSetItem(tableVideoRegisters, 7, 1, "%.2X", mem.read_byte(REGISTER_BGP));
	customSetItem(tableVideoRegisters, 8, 1, "%.2X", mem.read_byte(REGISTER_OBP0));
	customSetItem(tableVideoRegisters, 9, 1, "%.2X", mem.read_byte(REGISTER_OBP1));
	customSetItem(tableVideoRegisters, 10, 1, "%.2X", mem.read_byte(REGISTER_WY));
	customSetItem(tableVideoRegisters, 11, 1, "%.2X", mem.read_byte(REGISTER_WX));
	customSetItem(tableVideoRegisters, 12, 1, "%.2X", mem.read_byte(REGISTER_BCPS));
	customSetItem(tableVideoRegisters, 13, 1, "%.2X", mem.read_byte(REGISTER_BCPD));
	customSetItem(tableVideoRegisters, 14, 1, "%.2X", mem.read_byte(REGISTER_OCPS));
	customSetItem(tableVideoRegisters, 15, 1, "%.2X", mem.read_byte(REGISTER_OCPD));
}

#include "opcode.hpp"

void DbWindow::updateDisassembler(t_register& r, Memory& mem)
{
	uint8_t		opcode;
	uint8_t		data1;
	uint8_t		data2;
	uint16_t	pc;

	pc = r.PC;
	for (int i = 0; i < tableDisassembler->rowCount(); ++i)
	{
		char	buffer[32] = "%.2X";

		opcode				= mem.read_byte(pc);
		t_opcode&	instr	= _opcodeMap[opcode];

		if (instr.lengthData > 1) {
			data1 = mem.read_byte(pc + 1);
			sprintf(buffer, "%%.2X %.2X", data1);
		}
		if (instr.lengthData > 2) {
			data2 = mem.read_byte(pc + 2);
			sprintf(buffer, "%%.2X %.2X %.2X", data1, data2);
		}
		customSetItem(tableDisassembler, i, 0, "%.4X", pc);
		customSetItem(tableDisassembler, i, 1, instr.instructionName.c_str(), 0);
		customSetItem(tableDisassembler, i, 2, buffer , opcode);
		pc += instr.lengthData;
	}
}

void DbWindow::updateMemory(Memory& m)
{
	unsigned int row, col, curr;

	for (row = 0 ; row < 9 ; ++row) {
		curr = _start + row * 0x10;
		customSetItem(tableMemory, row - 1, col + 1, "%.4X:", curr);
		for (col = 0 ; col <= 0xF ; ++col)
			customSetItem(tableMemory, row, col + 1, "%.2X", m.read_byte(curr + col));
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

void	DbWindow::resetPressedSlot()
{
	emit	resetPressedSign();
}

void	DbWindow::openPressedSlot()
{
	QString path = QFileDialog::getOpenFileName(NULL, tr("Open XML File 1"), "/home", tr("XML Files (*.xml, *)"));

	emit	openPressedSign(path.toStdString());
}

void	DbWindow::bpAddPressedSlot()
{
	bool				ok;
	std::stringstream	ss;
	uint16_t			addr;
	QString text = QInputDialog::getText(this, tr("Breakpoint add"),
			                                   tr("Breakpoint addr:"), QLineEdit::Normal,
											   "0000", &ok);

	if (ok)
	{
		ss << std::hex << text.toStdString();
		ss >> addr;
		emit	bpAddSign(addr);
	}
}

// This function il call every 100ms see _timer 
void	DbWindow::updateAllSlot()
{
	updateRegister(*_r);
	updateOtherRegister(*_mem);
	updateMemory(*_mem);
	updateDisassembler(*_r, *_mem);
}

DbWindow::~DbWindow()
{
    delete ui;
}
