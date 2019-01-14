#pragma once
#include "ISubsystem.h"

//FMOD사용 클래스, sound의 적용과 재생만 관여
class Audio : public ISubsystem
{
public:
	Audio(class Context *context);
	virtual ~Audio();

	void Initialize() override;
	void Update();

	FMOD::System *GetFMODSystem() const { return system; }

private:
	FMOD::System *system; //FMOD(흰색) -> 네임스페이스, system이라는 이미 만들어진 함수가 있을 수 있어 따로 네임스페이스를 묶어줌
	int maxChannel; //최대 재생목록 수
	float distanceFactor; //거리 가중치
	bool bInitialized; //초기화 판별

	int Check; //에러 체크
};