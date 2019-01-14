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

	//FMOD ��ġ(system) ����
	Check = System_Create(&system);
	assert(Check == FMOD_OK);

	//FMOD ���� üũ
	uint version = 0;
	Check = system->getVersion(&version);
	assert(Check == FMOD_OK);
	assert(version >= FMOD_VERSION);

	//�ϵ����(���� ī�� ��ġ) ���� Ȯ��
	int driverCount = 0;
	Check = system->getNumDrivers(&driverCount);
	assert(Check == FMOD_OK);
	assert(driverCount > 0);

	//FMOD System �ʱ�ȭ
	Check = system->init(maxChannel, FMOD_INIT_NORMAL, nullptr);
	assert(Check == FMOD_OK);

	//FMOD 3D ����
	Check = system->set3DSettings(1.0f, distanceFactor, 0.0f);//���÷�, ,����ȿ��(ū�Ҹ����� ������ ũ��, �����Ҹ��� �۰�)
	assert(Check == FMOD_OK);
	bInitialized = true;
}

void Audio::Update()
{
	if (!bInitialized) return;
	Check = system->update();
	assert(Check == FMOD_OK);
}
