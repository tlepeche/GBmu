/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 14:10:07 by barbare           #+#    #+#             */
/*   Updated: 2016/11/03 10:34:49 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cpu.hpp"

/*
** ################################################################
** CREATE Singleton
** ################################################################
*/

Cpu::Cpu(unsigned int cpuAddrLenght,
			std::array<unsigned int, 4> arrFrequency,
			Register register) :	addrLenght(cpuAddrLenght),
									ptrAddr(0x00),
									_opcodeMap {
										{0x00, 0x00, 4 , 4 , 1, NULL},
										{0x01, 0x00, 12, 12, 3, NULL},
										{0x02, 0x00, 8 , 8 , 1, NULL},
										{0x03, 0x00, 8 , 8 , 1, NULL},
										{0x04, 0x00, 4 , 4 , 1, NULL},
										{0x05, 0x00, 4 , 4 , 1, NULL},
										{0x06, 0x00, 8 , 8 , 2, NULL},
										{0x07, 0x00, 4 , 4 , 1, NULL},
										{0x08, 0x00, 20, 20, 3, NULL},
										{0x09, 0x00, 8 , 8 , 1, NULL},
										{0x0a, 0x00, 8 , 8 , 1, NULL},
										{0x0b, 0x00, 8 , 8 , 1, NULL},
										{0x0c, 0x00, 4 , 4 , 1, NULL},
										{0x0d, 0x00, 4 , 4 , 1, NULL},
										{0x0e, 0x00, 8 , 8 , 2, NULL},
										{0x0f, 0x00, 4 , 4 , 1, NULL},
										{0x10, 0x00, 4 , 4 , 2, NULL},
										{0x11, 0x00, 12, 12, 3, NULL},
										{0x12, 0x00, 8 , 8 , 1, NULL},
										{0x13, 0x00, 8 , 8 , 1, NULL},
										{0x14, 0x00, 4 , 4 , 1, NULL},
										{0x15, 0x00, 4 , 4 , 1, NULL},
										{0x16, 0x00, 8 , 8 , 2, NULL},
										{0x17, 0x00, 4 , 4 , 1, NULL},
										{0x18, 0x00, 12, 12, 2, NULL},
										{0x19, 0x00, 8 , 8 , 1, NULL},
										{0x1a, 0x00, 8 , 8 , 1, NULL},
										{0x1b, 0x00, 8 , 8 , 1, NULL},
										{0x1c, 0x00, 4 , 4 , 1, NULL},
										{0x1d, 0x00, 4 , 4 , 1, NULL},
										{0x1e, 0x00, 8 , 8 , 2, NULL},
										{0x1f, 0x00, 4 , 4 , 1, NULL},
										{0x20, 0xc0, 12, 8 , 2, NULL},
										{0x21, 0x00, 12, 12, 3, NULL},
										{0x22, 0x00, 8 , 8 , 1, NULL},
										{0x23, 0x00, 8 , 8 , 1, NULL},
										{0x24, 0x00, 4 , 4 , 1, NULL},
										{0x25, 0x00, 4 , 4 , 1, NULL},
										{0x26, 0x00, 12, 12, 2, NULL},
										{0x27, 0x00, 4 , 4 , 1, NULL},
										{0x28, 0x80, 12, 8 , 2, NULL},
										{0x29, 0x00, 8 , 8 , 1, NULL},
										{0x2a, 0x00, 8 , 8 , 1, NULL},
										{0x2b, 0x00, 8 , 8 , 1, NULL},
										{0x2c, 0x00, 4 , 4 , 1, NULL},
										{0x2d, 0x00, 4 , 4 , 1, NULL},
										{0x2e, 0x00, 8 , 8 , 2, NULL},
										{0x2f, 0x00, 4 , 4 , 3, NULL},
										{0x30, 0x70, 12, 8 , 2, NULL},
										{0x31, 0x00, 12, 12, 3, NULL},
										{0x32, 0x00, 12, 12, 3, NULL},
										{0x33, 0x00, 8 , 8 , 1, NULL},
										{0x34, 0x00, 12, 12, 1, NULL},
										{0x35, 0x00, 12, 12, 1, NULL},
										{0x36, 0x00, 12, 12, 2, NULL},
										{0x37, 0x00, 4 , 4 , 1, NULL},
										{0x38, 0x10, 12, 8 , 2, NULL},
										{0x39, 0x00, 8 , 8 , 1, NULL},
										{0x3a, 0x00, 8 , 8 , 1, NULL},
										{0x3b, 0x00, 8 , 8 , 1, NULL},
										{0x3c, 0x00, 4 , 4 , 1, NULL},
										{0x3d, 0x00, 4 , 4 , 1, NULL},
										{0x3e, 0x00, 8 , 8 , 2, NULL},
										{0x3f, 0x00, 4 , 4 , 1, NULL},
										{0x40, 0x00, 4 , 4 , 1, NULL},
										{0x41, 0x00, 4 , 4 , 1, NULL},
										{0x42, 0x00, 4 , 4 , 1, NULL},
										{0x43, 0x00, 4 , 4 , 1, NULL},
										{0x44, 0x00, 4 , 4 , 1, NULL},
										{0x45, 0x00, 4 , 4 , 1, NULL},
										{0x46, 0x00, 8 , 8 , 1, NULL},
										{0x47, 0x00, 4 , 4 , 1, NULL},
										{0x48, 0x00, 4 , 4 , 1, NULL},
										{0x49, 0x00, 4 , 4 , 1, NULL},
										{0x4a, 0x00, 4 , 4 , 1, NULL},
										{0x4b, 0x00, 4 , 4 , 1, NULL},
										{0x4c, 0x00, 4 , 4 , 1, NULL},
										{0x4d, 0x00, 4 , 4 , 1, NULL},
										{0x4e, 0x00, 8 , 8 , 1, NULL},
										{0x4f, 0x00, 4 , 4 , 1, NULL},
										{0x50, 0x00, 4 , 4 , 1, NULL},
										{0x51, 0x00, 4 , 4 , 1, NULL},
										{0x52, 0x00, 4 , 4 , 1, NULL},
										{0x53, 0x00, 4 , 4 , 1, NULL},
										{0x54, 0x00, 4 , 4 , 1, NULL},
										{0x55, 0x00, 4 , 4 , 1, NULL},
										{0x56, 0x00, 8 , 8 , 1, NULL},
										{0x57, 0x00, 4 , 4 , 1, NULL},
										{0x58, 0x00, 4 , 4 , 1, NULL},
										{0x59, 0x00, 4 , 4 , 1, NULL},
										{0x5a, 0x00, 4 , 4 , 1, NULL},
										{0x5b, 0x00, 4 , 4 , 1, NULL},
										{0x5c, 0x00, 4 , 4 , 1, NULL},
										{0x5d, 0x00, 4 , 4 , 1, NULL},
										{0x5e, 0x00, 8 , 8 , 1, NULL},
										{0x5f, 0x00, 4 , 4 , 1, NULL},
										{0x60, 0x00, 4 , 4 , 1, NULL},
									}
}
									register(register)
{
	regInProgress.execute = true;
}

Cpu_z80 &Cpu_z80::Instance(void)
{
	return this->_instance;
}

	/*
	** ################################################################
	** METHOD opcode
	** ################################################################
	*/

t_opcode Cpu::_getOpcode(uint8_t opcode)
{
	unsigned int i = 0;

	while (i < NB_OPCODE)
	{
		if (this->opcodeMap[i].opcode == opcode)
			return this->opcodeMap[i];
		++i;
	}
	return this->opcodeMap[0];
}

uint8_t	Cpu::getCylceOpcode()
{
	if (this->register.F & mask > 0)
		return this->cycleOpcodeFlag;
	return cycleOpcodeNoFlag;
}


unsigned int Cpu::getCycleOpcode(void)
{
	if (this->regInProgress.execute == true)
		this->regInProgress = this->getOpcode;
	return this->regInProgress.reg.cycleOpcode;
}

void *Cpu::getFunctionOpcode(void)
{
	if (this->regInProgress.execute == true)
		this->regInProgress = this->getOpcode;
	return this->regInProgress.reg.functionOpcode;
}

unsigned int Cpu::getDataOpcode(void) //TODO: can create type 8b or 16b data, moment use uint
{
	if (this->regInProgress.execute == true)
		this->regInProgress = this->getOpcode;
	return this->regInProgress.reg.data;
}

unsigned int Cpu::getLenghtDataOpcode(void)
{
	if (this->regInProgress.execute == true)
		this->regInProgress = this->getOpcode;
	return this->regInProgress.reg.lenghtData;
}
unsigned int Cpu::nbCycleNextOpCode(void)
{
	return getCycleOpcode(opcode);
}

unsigned int Cpu::executeNextOpcode(void)
{
	getFunctionOpcode()(this->getDataOpcode());
	unsigned int cycle = this->getCycleOpcode();
	this->_nextPtr();
	this->regInProgress.execute = true;
	return cycle;
}

void Cpu::_nextPtr(void)
{
	this->_ptrAddr >> (addressLength + this->getLenghtDataOpcode());
}


void Cpu::interrupt(void)
{
}

std::array<unsigned int, 4> Cpu::getArrayFrequency()
{
	return this->_arrayFrequency;
}

unsigned int Cpu::_getNextOpcode()
{
	return (this->_ram.read8b(this->_ptrAddr));
}

void Cpu::_resetPtrAddr(void)
{
	this->_ptrAddr = 0x00;
}
