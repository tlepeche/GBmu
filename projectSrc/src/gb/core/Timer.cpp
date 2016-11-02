/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 14:08:29 by barbare           #+#    #+#             */
/*   Updated: 2016/10/27 20:27:19 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ############################################################################
** CREATE Singleton
** ############################################################################
*/
static Timer::Timer g_Timer = new Timer();

Timer::Timer &Timer::Instance(void)
{
	return &g_Timer;
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

void Timer::setFrequency(const std::array<int, 4> arrFrequency)
{
	this->_arrayFrequency = arrFrequency;
}

void Timer::setCycleTotal(unsigned int cycleTotal)
{
	this->_cyclesTotal = cycleTotal;
}

void Timer::setCycleAcc(unsigned int cycles)
{
	this->_cycle += cycles
}
_
/*
** ############################################################################
** Methodes GETTEUR
** ############################################################################
*/
unsigned int Timer::getCycleAcc(void)
{
	return (this->_getCycleOpcodeTotal() - this->_cycles);
}

unsigned int Timer::getArrayFrequency(const unsigned int idFrequency)
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

unsigned int _getCycleOpcodeTotal(void)
{
	return (this->_cyclesTotal);
}

