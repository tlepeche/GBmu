#include "Timer.hpp"
#include "Cpu.hpp"
#include "registerAddr.hpp"

/*
** ############################################################################
** CREATE Singleton
** ############################################################################
*/

Timer::Timer(Memory *memory) :
	_memory(memory),
	_cycles(0),
	_cyclesAcc(0)
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

void	Timer::step(unsigned int cycles)
{
	uint8_t			tac = _memory->read_byte(REGISTER_TAC) & 0x3;
	unsigned int	cyclesAccTima = Cpu_z80::getClockSpeed() / _arrayFrequency[tac];

	_cyclesAcc += cycles;
	if (_cyclesAcc >= cyclesAccTima)
	{
		incTima();
		_cyclesAcc -= cyclesAccTima;
	}
}

void	Timer::incTima(void)
{
	uint8_t			tima = _memory->read_byte(REGISTER_TIMA);
	uint8_t			tma = _memory->read_byte(REGISTER_TMA);

	if (tima == 0xFF)
		_memory->write_byte(REGISTER_TIMA, tma);
	else
		_memory->write_byte(REGISTER_TIMA, tima + 1);
}
