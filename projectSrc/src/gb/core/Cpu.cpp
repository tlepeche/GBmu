/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 14:10:07 by barbare           #+#    #+#             */
/*   Updated: 2016/10/27 19:04:53 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cpu.hpp"

Cpu::Cpu(unsigned int cpuAddrLenght,
			std::array<unsigned int, 4> arrFrequency,
			Register register) :	addrLenght(cpuAddrLenght),
									ptrAddr(0x00),
									register(register)
{
	regInProgress.execute = true;
}

s_opcode Cpu::getOpcode(unsigned int opcode)
{
	unsigned int i = 0;
	s_opcode newOpcode;

	bzero(newOpcode, sizeof(newOpcode));
	while (register.getReg()[i])
	{
		if (register.getOpcode(register.getReg()[i]) == opcode)
		{
			newOpcode.execute = false;
			newOpcode.opcode.opcode = opcode;
			newOpcode.opcode.functionOpcode = register.getFunctionOpcode(register.getReg()[i]);
			newOpcode.opcode.cycleOpcode = register.getCycleOpcode(register.getReg()[i]);
			newOpcode.opcode.data = register.getDataOpcode(register.getReg()[i]);
			newOpcode.opcode.lenghtData = register.getLenghtDataOpcode(register.getReg()[i]);
			return (newreg);
		++i;
	}
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
