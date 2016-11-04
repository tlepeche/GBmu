#include "Cpu.hpp"
#include "stdio.h"

/*
** ################################################################
** CREATE Singleton
** ################################################################
*/
Cpu_z80		Cpu_z80::_instance = Cpu_z80();

Cpu_z80::Cpu_z80(void) :
						_memory(Memory::Instance())
{
}

Cpu_z80::~Cpu_z80(void)
{
}

Cpu_z80 &Cpu_z80::Instance(void)
{
	return Cpu_z80::_instance;
}

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

t_opcode Cpu_z80::_getOpcode(uint8_t opcode)
{
	uint8_t i = 0;

	while (i < NB_OPCODE)
	{
		if (_opcodeMap[i].opcode == opcode)
			return _opcodeMap[i];
		++i;
	}
	return _opcodeMap[0];
}

uint8_t	Cpu_z80::_getCycleOpcode(void)
{
	//if (this->register.F & mask > 0)
	//	return this->_opcodeInProgress.cycleOpcodeFlag;
	return this->_opcodeInProgress.cycleOpcodeNoFlag > this->_opcodeInProgress.cycleOpcodeFlag ?
				this->_opcodeInProgress.cycleOpcodeNoFlag : this->_opcodeInProgress.cycleOpcodeFlag;
}


void Cpu_z80::_setDataOpcode(void)
{
	if (this->_opcodeInProgress.lengthData > 1)
	{
		if (this->_opcodeInProgress.lengthData > 2)
			this->_opcodeInProgress.data = this->_memory.read_byte(this->_cpuRegister.PC >> sizeof(uint8_t));
		else
			this->_opcodeInProgress.data = this->_memory.read_word(this->_cpuRegister.PC >> sizeof(uint8_t));
	}
}

uint16_t Cpu_z80::_getDataOpcode(void) //TODO: can create type 8b or 16b data, moment use uint
{
	return this->_opcodeInProgress.data;
}

uint8_t Cpu_z80::_getLengthDataOpcode(void)
{
	return this->_opcodeInProgress.lengthData;
}

void Cpu_z80::_nextPtr(void)
{
	this->_cpuRegister.PC = this->_cpuRegister.PC >> this->_opcodeInProgress.lengthData * sizeof(uint8_t);
	this->_opcodeInProgress = this->_getOpcode(this->_memory.read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();
}

uint8_t Cpu_z80::nbCycleNextOpCode(void)
{
	return this->_getCycleOpcode();
}

uint8_t Cpu_z80::executeNextOpcode(void)
{
	this->_opcodeInProgress.functionOpcode();
	uint8_t cycle = this->_getCycleOpcode();
	this->_nextPtr();
	return cycle;
}

void Cpu_z80::initIOFlags(void)
{
	this->_memory.reset();
	this->_cpuRegister.PC = 0x100;
	this->_setHightBit(REGISTER_TAC, 2);
	this->_opcodeInProgress = this->_getOpcode(this->_memory.read_byte(this->_cpuRegister.PC));
	this->_setDataOpcode();
}

/*
** ################################################################
** METHOD Math
** ################################################################
*/

void Cpu_z80::_setLowBit(uint16_t addr, uint8_t bit)
{
	this->_memory.write_byte(addr, (uint8_t)((0x00 << bit) & this->_memory.read_byte(addr)));
}

void Cpu_z80::_setHightBit(uint16_t addr, uint8_t bit)
{
	this->_memory.write_byte(addr, (uint8_t)((0x01 << bit) | this->_memory.read_byte(addr)));
}

void Cpu_z80::interrupt(void)
{
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
