#include "Timer.hpp"
#include <unistd.h>

/*
** ############################################################################
** CREATE Singleton
** ############################################################################
*/

Timer		Timer::_instance = Timer();

Timer &Timer::Instance(void)
{
	return Timer::_instance;
}

Timer::Timer(void)
{
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

void Timer::setFrequency(const std::array<uint8_t, 4> arrFrequency)
{
	this->_arrayFrequency = arrFrequency;
}

void Timer::setCycleTotal(uint8_t cycleTotal)
{
	this->_cyclesTotal = cycleTotal;
}

void Timer::setCycleAcc(uint8_t cycles)
{
	this->_cycles += cycles;
}

/*
** ############################################################################
** Methodes GETTEUR
** ############################################################################
*/
uint8_t Timer::getCycleAcc(void)
{
	return (this->_getCycleOpcodeTotal() - this->_cycles);
}

uint8_t Timer::getArrayFrequency(const uint8_t idFrequency)
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

uint8_t Timer::_getCycleOpcodeTotal(void)
{
	return (this->_cyclesTotal);
}

