#pragma once
#include "ISubsystem.h"

class Timer : public ISubsystem
{
public:
	Timer(class Context *context);
	virtual ~Timer();

	void Initialize() override;
	void Update();
	
	const float &GetDeltaTimeMs() const { return deltaTimeMs; }
	const float &GetDeltaTimeSec() const { return deltaTimeSec; }
private:
	chrono::high_resolution_clock::time_point prevTime;

	float deltaTimeMs;
	float deltaTimeSec;

};