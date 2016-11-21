#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <list>
#include <QDialog>

namespace Ui {
	class DbWindow;
}

class QTableWidget;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class Memory;
typedef struct s_register t_register;

class DbWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DbWindow(t_register *r, Memory *mem, std::list<uint16_t> *_breakpoints);
    virtual	~DbWindow();

	void 	updateRegister(t_register& r);
	void	updateOtherRegister(Memory& mem);
	void 	updateMemory(Memory& m);
	void	updateDisassembler(t_register& r, Memory& mem);

	void	bpDel(QListWidgetItem *item);

	QTableWidget	*tableRegisters;
	QTableWidget	*tableOtherRegisters;
	QTableWidget	*tableVideoRegisters;
	QTableWidget	*tableDisassembler;
	QTableWidget	*tableMemory;

	QListWidget		*listBreakpoint;

	QPushButton		*buttonReset;
	QPushButton		*buttonStep;
	QPushButton		*buttonFrame;
	QPushButton		*buttonRun;
	QPushButton		*buttonOpen;
	QPushButton		*buttonBpAdd;
	QPushButton		*okEdit;

	QLineEdit		*lineAddr;
	QLineEdit		*lineStepCount;
	QLineEdit		*addrEdit;
	QLineEdit		*valEdit;


	signals:
		void	stepPressedSign(unsigned int count);
		void	framePressedSign();
		void	runPressedSign();
		void	resetPressedSign();
		void	openPressedSign(std::string path);
		void	bpAddSign(uint16_t addr);
		void	bpDelSign(uint16_t addr);
	public slots:
		void	stepPressedSlot();
		void	lineStepCountEditedSlot();
		void	framePressedSlot();
		void	runPressedSlot();
		void	resetPressedSlot();
		void	openPressedSlot();
		void	bpAddPressedSlot();
		void	bpDoubleClikedSlot(QListWidgetItem *item);
		void	updateAllSlot();
		void	lineAddrEditedSlot();
		void	editAddrSlot();

private:
    Ui::DbWindow	*ui;

	t_register			*_r;
	Memory				*_mem;
	std::list<uint16_t>	*_breakpoints; // unuse at this time
	unsigned int		_start;
	unsigned int		_stepCount;
};

#endif // DBWINDOW_H
