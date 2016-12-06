
#include "Audio.hpp"
#include <iostream>

Audio::Audio() :
	_enabled(true),
	_Time(0),
	_AbsoluteTime(0),
	_iSampleRate(44100),
	_Apu(nullptr),
	_Buffer(nullptr),
	_Sound(nullptr),
	_SampleBuffer(nullptr)

{
	init();
}

Audio::~Audio()
{
    delete		_Apu;
    delete		_Buffer;
    delete		_Sound;
    delete[]	_SampleBuffer;
}

void Audio::init()
{
    int error = SDL_Init(SDL_INIT_AUDIO);
    
    if (error < 0)
    {
		std::cerr <<  "--> ** (" << error << ") SDL Audio not initialized: " << SDL_GetError() << std::endl;
    }

    atexit(SDL_Quit);

    _SampleBuffer = new blip_sample_t[sample_buffer_size];

    _Apu = new Gb_Apu();
    _Buffer = new Stereo_Buffer();
    _Sound = new Sound_Queue();

    _Buffer->clock_rate(4194304);
    _Buffer->set_sample_rate(_iSampleRate);

    _Apu->treble_eq(-15.0);
    _Buffer->bass_freq(100);

    _Apu->set_output(_Buffer->center(), _Buffer->left(), _Buffer->right());

    _Sound->start(_iSampleRate, 2);
}

void Audio::reset(bool bCGB)
{
	Gb_Apu::mode_t mode = bCGB ? Gb_Apu::mode_cgb : Gb_Apu::mode_dmg;
	_Apu->reset(mode);
	_Buffer->clear();

	_Time = 0;
	_AbsoluteTime = 0;

	_Sound->stop();
	_Sound->start(_iSampleRate, 2);
}

void Audio::enable(bool enabled)
{
    _enabled = enabled;
}

bool Audio::isEnabled() const
{
    return _enabled;
}

void Audio::setSampleRate(int rate)
{
    if (rate != _iSampleRate)
    {
        _iSampleRate = rate;
        _Buffer->set_sample_rate(_iSampleRate);
        _Sound->stop();
        _Sound->start(_iSampleRate, 2);
    }
}

void Audio::endFrame()
{
    _Apu->end_frame(_AbsoluteTime);
    _Buffer->end_frame(_AbsoluteTime);

    if (_Buffer->samples_avail() >= sample_buffer_size)
    {
        long count = _Buffer->read_samples(_SampleBuffer, sample_buffer_size);
        if (_enabled)
        {
            _Sound->write(_SampleBuffer, (int)count);
        }
    }
}
