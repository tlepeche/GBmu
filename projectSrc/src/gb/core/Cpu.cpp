#include "Cpu.hpp"
#include "registerAddr.hpp"
#include <stdio.h>

/*
 ** ################################################################
 ** CREATE Singleton
 ** ################################################################
 */
// See opcode.hpp
std::array<t_opcode, 256>		_opcodeMap;
std::array<t_opcode, 256>		_CBopcodeMap;

Cpu_z80::Cpu_z80(Memory *memory) :
	_memory(memory)
{
	_setOpcodeMap();
	_setCbOpcodeMap();
}

Cpu_z80::~Cpu_z80(void)
{
}

/*
 ** ################################################################
 ** METHOD opcode
 ** ################################################################
*/

t_opcode Cpu_z80::_getOpcode(uint8_t opcode)
{
	if (opcode == 0xCB)
	{
		this->_cpuRegister.PC += LENGTH_ADDR;
		uint8_t cbopcode = this->_memory->read_byte(this->_cpuRegister.PC);
		return _CBopcodeMap[cbopcode];
	}
	return _opcodeMap[opcode];
}

uint8_t	Cpu_z80::_getCycleOpcode(void)
{
	return this->_opcodeInProgress.cycleOpcodeNoFlag > this->_opcodeInProgress.cycleOpcodeFlag ?
		this->_opcodeInProgress.cycleOpcodeNoFlag : this->_opcodeInProgress.cycleOpcodeFlag;
}


void Cpu_z80::_setDataOpcode(void)
{
	if (this->_opcodeInProgress.lengthData > 1 && getStepState())
	{
		if (this->_opcodeInProgress.lengthData > 2)
			this->_opcodeInProgress.data = this->_memory->read_byte(this->_cpuRegister.PC + LENGTH_ADDR);
		else
			this->_opcodeInProgress.data = this->_memory->read_word(this->_cpuRegister.PC + LENGTH_ADDR);
	}
}

void Cpu_z80::setStepState(bool state)
{
	_stepState = state;
}

bool Cpu_z80::getStepState()
{
	return (this->_stepState);
}

uint16_t Cpu_z80::_getDataOpcode(void)
{
	return this->_opcodeInProgress.data;
}

uint8_t Cpu_z80::_getLengthDataOpcode(void)
{
	return this->_opcodeInProgress.lengthData;
}

void Cpu_z80::_setHalt(bool state)
{
	this->_halt = state;
}

bool Cpu_z80::getHalt(void)
{
	return this->_halt;
}

void Cpu_z80::_setStop(bool state)
{
	this->_stop = state;
}

bool Cpu_z80::getStop(void)
{
	return this->_stop;
}

void Cpu_z80::_setIME(bool state)
{
	this->_IME = state;
}

void Cpu_z80::setHoldIME(bool state)
{
	this->_holdIME = state;
}

bool Cpu_z80::getHoldIME(void)
{
	return this->_holdIME;
}

bool Cpu_z80::getIME(void)
{
	return this->_IME;
}

bool Cpu_z80::isInterrupt(void)
{
	return (this->_memory->read_byte(REGISTER_IF) > 0x00);
}

bool Cpu_z80::_getInterrupt(uint8_t interrupt)
{
	return ((this->_memory->read_byte(0xFF0F) & interrupt) >= 0x1);
}

void Cpu_z80::_nextPtr(void) {
	if (getStepState())
		this->_cpuRegister.PC = this->_cpuRegister.PC + this->_opcodeInProgress.lengthData;
	this->_opcodeInProgress = this->_getOpcode(this->_memory->read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();
}

uint8_t Cpu_z80::nbCycleNextOpCode(void)
{
	return this->_getCycleOpcode();
}

uint8_t Cpu_z80::executeNextOpcode(void)
{
	if (this->_opcodeInProgress.functionOpcode == NULL)
		printf("Function not yet implemented: opcode(%.2X), PC = 0x%02x\n", this->_opcodeInProgress.opcode, _cpuRegister.PC);
	else
		this->_opcodeInProgress.functionOpcode();
	uint8_t cycle = this->_getCycleOpcode();
	this->_nextPtr();
	setStepState(true);
	return cycle;
}

#include "registerAddr.hpp"

void Cpu_z80::init(void)
{
	setStepState(true);
	printf("INITIALIZING\n");
	this->_IME = true;
	this->_holdIME = true;
	htype typeRom;

	//init register cpu
	//if (this->_rom.isLoaded()) //TODO: when david PR his code
	typeRom = GB; //this->_rom.getType().type;


	//TODO: A modifier pour rajouter le forcage de type GB GBC
	this->_cpuRegister.A = typeRom == GB ? 0x01 : 0x11;
	this->_cpuRegister.F = 0xB0;
	this->_cpuRegister.BC = 0x0013;
	this->_cpuRegister.DE = 0x00D8;
	this->_cpuRegister.HL = 0x014D;

	this->_cpuRegister.PC = 0x100;
	this->_cpuRegister.SP = 0xFFFE;

	//init register memory
	this->_memory->write_byte(REGISTER_NR10, 0x80);
	this->_memory->write_byte(REGISTER_NR11, 0xBF);
	this->_memory->write_byte(REGISTER_NR12, 0xF3);
	this->_memory->write_byte(REGISTER_NR14, 0xBF);
	this->_memory->write_byte(REGISTER_NR21, 0x3F);
	this->_memory->write_byte(REGISTER_NR22, 0x00);
	this->_memory->write_byte(REGISTER_NR24, 0xBF);
	this->_memory->write_byte(REGISTER_NR30, 0x7F);
	this->_memory->write_byte(REGISTER_NR31, 0xFF);
	this->_memory->write_byte(REGISTER_NR32, 0x9F);
	this->_memory->write_byte(REGISTER_NR33, 0xBF);
	this->_memory->write_byte(REGISTER_NR41, 0xFF);
	this->_memory->write_byte(REGISTER_NR42, 0x00);
	this->_memory->write_byte(REGISTER_NR43, 0x00);
	this->_memory->write_byte(REGISTER_NR30_, 0xBF);
	this->_memory->write_byte(REGISTER_NR50, 0x77);
	this->_memory->write_byte(REGISTER_NR51, 0xF3);
	this->_memory->write_byte(REGISTER_NR52, 0xF1);

	// Other register
	this->_memory->write_byte(REGISTER_P1, 0xCF);
	this->_memory->write_byte(REGISTER_SB, 0x00);
	this->_memory->write_byte(REGISTER_SC, 0x7E);
	this->_memory->write_byte(REGISTER_DIV, 0xD3); // bios: 0xD3 start: 0x81
	this->_memory->write_byte(REGISTER_TIMA, 0x00);
	this->_memory->write_byte(REGISTER_TMA, 0x00);
	this->_memory->write_byte(REGISTER_TAC, 0x00);
	this->_memory->write_byte(REGISTER_KEY1, 0xFF);
	this->_memory->write_byte(REGISTER_VBK, 0xFF);
	this->_memory->write_byte(REGISTER_HDMA1, 0xFF);
	this->_memory->write_byte(REGISTER_HDMA2, 0xFF);
	this->_memory->write_byte(REGISTER_HDMA3, 0xFF);
	this->_memory->write_byte(REGISTER_HDMA4, 0xFF);
	this->_memory->write_byte(REGISTER_HDMA5, 0xFF);
	this->_memory->write_byte(REGISTER_SVBK, 0xFF);
	this->_memory->write_byte(REGISTER_IF, 0xE1);
	this->_memory->write_byte(REGISTER_IE, 0x00);
	

	this->_memory->write_byte(REGISTER_LCDC, 0x91);
	this->_memory->write_byte(REGISTER_STAT, 0x81); // bios: 0x80 start: 0x81
	this->_memory->write_byte(REGISTER_SCY, 0x00);
	this->_memory->write_byte(REGISTER_SCX, 0x00);
	this->_memory->write_byte(REGISTER_LY, 0x00); // bios: 0x00 start: 0x81
	this->_memory->write_byte(REGISTER_LYC, 0x00);
	this->_memory->write_byte(REGISTER_DMA, 0xFF);
	this->_memory->write_byte(REGISTER_BGP, 0xFC); // edelangh: this is bullshit !!
	this->_memory->write_byte(REGISTER_OBP0, 0xFF);
	this->_memory->write_byte(REGISTER_OBP1, 0xFF);
	this->_memory->write_byte(REGISTER_WY, 0x00);
	this->_memory->write_byte(REGISTER_WX, 0x00);
	/* TODO: WTF ? I dont see then in doc */
	this->_memory->write_byte(REGISTER_BCPS, 0xFF);
	this->_memory->write_byte(REGISTER_BCPD, 0xFF);
	this->_memory->write_byte(REGISTER_OCPS, 0xFF);
	this->_memory->write_byte(REGISTER_OCPD, 0xFF);

	this->_opcodeInProgress = this->_getOpcode(this->_memory->read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();

	//TODO: Setup loop, i think this one is up when rom memory is plugged
	this->_setHightBit(REGISTER_TAC, 2);
}

/*
 ** ################################################################
 ** METHOD Math
 ** ################################################################
 */

void Cpu_z80::_setLowBit(uint16_t addr, uint8_t bit)
{
	this->_memory->write_byte(addr, (uint8_t)((0x01 << bit) ^ this->_memory->read_byte(addr)));
}

void Cpu_z80::_setHightBit(uint16_t addr, uint8_t bit)
{
	this->_memory->write_byte(addr, (uint8_t)((0x01 << bit) | this->_memory->read_byte(addr)));
}

void Cpu_z80::execInterrupt(void)
{
	if (this->_getInterrupt(0x4))
	{
		this->_setHalt(false);
		this->_setStop(false);
	}
	else if (this->isInterrupt())
		this->_setHalt(false);
	std::cout << "interrupt cpu" << std::endl;
}

std::array<uint32_t, 4> Cpu_z80::getArrayFrequency()
{
	return this->_arrayFrequency;
}

void Cpu_z80::_resetPtrAddr(void)
{
	this->_cpuRegister.PC = 0x100;
}
