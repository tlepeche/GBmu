#include "Timer.hpp"
#include "registerAddr.hpp"
#include <unistd.h>

/*
** ############################################################################
** CREATE Singleton
** ############################################################################
*/

Timer::Timer(Memory *memory) : _memory(memory)
{
	this->_memory->write_byte(REGISTER_TIMA, 0x00);
	this->_memory->write_byte(REGISTER_TMA, 0x00);
	this->_memory->write_byte(REGISTER_TAC, 0x00);
	this->reset();
}

/*
** ############################################################################
** Methodes SETTEUR
** ############################################################################
*/

void Timer::reset(void)
{
	this->_cycles = 0;
}

void Timer::setFrequency(const std::array<uint32_t, 4> arrFrequency)
{
	this->_arrayFrequency = arrFrequency;
}

void Timer::setCycleTotal(uint32_t cycleTotal)
{
	this->_cyclesTotal = cycleTotal;
}

#include <inttypes.h>
void Timer::setCycleAcc(uint8_t cycles)
{
	static uint64_t test = 0;
	test += cycles;
	//printf("cycles consummed: %"PRIu64"\n", test);
	_doDividerRegister(cycles);
	_doClockRegister(cycles);
}

void Timer::_doDividerRegister(uint8_t cycles)
{
	this->_divider += cycles;
	if (this->_divider >= 0xFF)
	{
		this->_divider = 0x00;
		this->_memory->write_byte(REGISTER_DIV, this->_memory->read_byte(REGISTER_DIV) + 0x01);
	}
}

void Timer::_doClockRegister(uint8_t cycles)
{
	this->_cycles += cycles;
	if (this->_cycles >= this->_cyclesTotal)
	{
		this->_memory->write_byte(REGISTER_TIMA, (this->_memory->read_byte(REGISTER_TIMA) + 1));
		this->_cycles = 0x00;
	}
	if (this->_memory->read_byte(REGISTER_TIMA) >= 0xFF)
	{
		this->_memory->write_byte(REGISTER_TIMA, 0x00);
		this->_memory->write_byte(REGISTER_IF, (uint8_t)((0x01 << 2) | this->_memory->read_byte(REGISTER_IF)));
	}
}

/*
** ############################################################################
** Methodes GETTEUR
** ############################################################################
*/

bool Timer::isCycleAcc(uint32_t cycle)
{
	if (this->_cycles + cycle > this->getCycleAcc())
	{
		this->_memory->write_byte(REGISTER_TIMA, (this->_memory->read_byte(REGISTER_TIMA) + 1));
		this->_cycles = 0x00;
		return false;
	}
	return true;
}

uint32_t Timer::getCycleAcc(void)
{
	return (this->_getCycleOpcodeTotal() - this->_cycles);
}

uint32_t Timer::getArrayFrequency(const uint8_t idFrequency)
{
	return (_arrayFrequency[idFrequency]);
}

/*
** ############################################################################
** PRIVATE Function
** ############################################################################
*/

void Timer::sleep(unsigned char ms)
{
	usleep(ms);
}

uint32_t Timer::_getCycleOpcodeTotal(void)
{
	return (this->_cyclesTotal);
}

