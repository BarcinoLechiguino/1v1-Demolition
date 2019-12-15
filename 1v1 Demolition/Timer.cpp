// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
	: running(false)
	, started_at(0)
	, stopped_at(0)
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return (SDL_GetTicks() - started_at) / 1000.0f;		//Returns the amount of time that has passed from Start method call until this method's call. As data is received in ms, it is converted to seconds.
}


