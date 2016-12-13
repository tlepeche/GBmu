
#ifndef AUDIO_H
#define	AUDIO_H

#include "audio/Multi_Buffer.h"
#include "audio/Gb_Apu.h"
#include "audio/Sound_Queue.h"

class Audio
{
	public:
		Audio();
		~Audio();

		void		init();
		void		reset(bool bCGB);
		void		enable(bool enabled);
		bool		isEnabled() const;
		void		setSampleRate(int rate);
		uint8_t		read_register(uint16_t address);
		void		write_register(uint16_t address, uint8_t value);
		void		endFrame();
		void		step(unsigned int clockCycles);
		void		setSpeed(uint8_t speed);

	private:
		bool			_enabled;
		int				_Time;
		int				_AbsoluteTime;
		int				_iSampleRate;
		uint8_t			_speed;
		Gb_Apu			*_Apu;
		Stereo_Buffer	*_Buffer;
		Sound_Queue		*_Sound;
		blip_sample_t	*_SampleBuffer;
};

const int sample_buffer_size = 2048;
const long frame_length = 10000;

inline void Audio::step(unsigned int clockCycles)
{
	_Time += clockCycles;
	_AbsoluteTime += clockCycles;

	if (_Time >= frame_length)
	{
		endFrame();
		_Time -= frame_length;
		_AbsoluteTime = 0;
	}
}

inline uint8_t Audio::read_register(uint16_t address)
{
	return _Apu->read_register(_Time, address);
}

inline void Audio::write_register(uint16_t address, uint8_t value)
{
	_Apu->write_register(_Time, address, value);
}

#endif
