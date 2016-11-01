#include <iostream>
#include "memory.hpp"

Memory		Memory::_instance = Memory();

Memory::Memory(void)
{
	this->_memory_size = 0;
	this->_memory = NULL;
}

Memory::~Memory(void)
{
	if (this->_memory != NULL)
		delete[] this->_memory;
}

void			Memory::Init(uint32_t size)
{
	if (Memory::_instance._memory == NULL && Memory::_instance._memory_size == 0)
		Memory::_instance.SetMemory(size);
}

Memory			&Memory::Instance(void)
{
	return Memory::_instance;
}

void			Memory::SetMemory(uint32_t size)
{
	if (size > 0xFFFF)
		this->_mask = 0x1FFFF;
	else
		this->_mask = 0xFFFF;
	this->_memory_size = size;
	this->_memory = new char [size + 1];
}

void			Memory::reset(void)
{
	int		i;

	for (i = 0; i < this->_memory_size; ++i)
		this->_memory[i] = 0;
}

uint8_t			Memory::read_byte(uint32_t addr)
{
	return this->_memory[addr & this->_mask];
}

void			Memory::write_byte(uint32_t addr, uint8_t val)
{
	this->_memory[addr & this->_mask] = (val & 0xFF);
}

uint16_t		Memory::read_word(uint32_t addr)
{
	return Memory::_memory[addr & this->_mask] + (this->_memory[(addr + 1) & this->_mask] << 8);
}

void			Memory::write_word(uint32_t addr, uint16_t val)
{
	this->_memory[addr & this->_mask] = (val & 0xFF);
	this->_memory[(addr + 1) & this->_mask] = ((val & 0xFF00) >> 8);
}
