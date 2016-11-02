#ifndef _TIMER_HPP
# define _TIMER_HPP


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
		Timer(void) : _cycles(0);
		~Timer(void);

	/*
	** ################################################################
	** SETTEUR
	** ################################################################
	*/
	public:
		void reset();
		void setCycleAcc(unsigned int cycle);
		void setFrequency(const std::array<int, 4> arrFrequency);
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
		unsigned int _getCycleOpcodeTotal(void)

	/*
	** ################################################################
	** ATTRIBUTE
	** ################################################################
	*/
	private:
		std::array<int, 4> _arrayFrequency;
		unsigned int _cycles;
		const unsigned int _cyclesTotal;

}

#endif
