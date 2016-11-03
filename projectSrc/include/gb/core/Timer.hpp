#ifndef _TIMER_HPP
# define _TIMER_HPP

#include <iostream>
#include <array>

class Timer
{
	/*
	** ################################################################
	** CREATE Singleton
	** ################################################################
	*/
	public:
		Timer &Instance();

	private:
		Timer(void);
		~Timer(void);

	/*
	** ################################################################
	** SETTEUR
	** ################################################################
	*/
	public:
		void reset();
		void setCycleAcc(unsigned int cycle);
		void setFrequency(std::array<int, 4> arrFrequency);
		void setCycleTotal(unsigned int cycle);

	/*
	** ################################################################
	** GETTEUR
	** ################################################################
	*/
	public:
		unsigned int getArrayFrequency(const unsigned int idFrequency);
		unsigned int getCycleAcc(void);

	/*
	** ################################################################
	** METHOD
	** ################################################################
	*/
	public:
		void sleep(unsigned char ms);

	private:
		unsigned int _getCycleOpcodeTotal(void);

	/*
	** ################################################################
	** ATTRIBUTE
	** ################################################################
	*/
	private:
		std::array<int, 4> _arrayFrequency;
		unsigned int _cycles;
		unsigned int _cyclesTotal;
};

#endif
