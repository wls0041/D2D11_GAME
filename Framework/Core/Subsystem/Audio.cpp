#include "stdafx.h"
#include "Audio.h"

Audio::Audio(Context * context) : ISubsystem(context), system(nullptr), maxChannel(32), distanceFactor(1.0f), bInitialized(false), Check(FMOD_OK)
{
}

Audio::~Audio()
{
	if (!bInitialized) return;
	Check = system->close();
	assert(Check == FMOD_OK);

	Check = system->release();
	assert(Check == FMOD_OK);
}

void Audio::Initialize()
{
	if (bInitialized) return;

	//FMOD 장치(system) 생성
	Check = System_Create(&system);
	assert(Check == FMOD_OK);

	//FMOD 버전 체크
	uint version = 0;
	Check = system->getVersion(&version);
	assert(Check == FMOD_OK);
	assert(version >= FMOD_VERSION);

	//하드웨어(사운드 카드 장치) 유무 확인
	int driverCount = 0;
	Check = system->getNumDrivers(&driverCount);
	assert(Check == FMOD_OK);
	assert(driverCount > 0);

	//FMOD System 초기화
	Check = system->init(maxChannel, FMOD_INIT_NORMAL, nullptr);
	assert(Check == FMOD_OK);

	//FMOD 3D 세팅
	Check = system->set3DSettings(1.0f, distanceFactor, 0.0f);//도플러, ,감쇄효과(큰소리에선 낙차폭 크게, 작은소리는 작게)
	assert(Check == FMOD_OK);
	bInitialized = true;
}

void Audio::Update()
{
	if (!bInitialized) return;
	Check = system->update();
	assert(Check == FMOD_OK);
}
