#pragma once
#include "ISubsystem.h"

//FMOD��� Ŭ����, sound�� ����� ����� ����
class Audio : public ISubsystem
{
public:
	Audio(class Context *context);
	virtual ~Audio();

	void Initialize() override;
	void Update();

	FMOD::System *GetFMODSystem() const { return system; }

private:
	FMOD::System *system; //FMOD(���) -> ���ӽ����̽�, system�̶�� �̹� ������� �Լ��� ���� �� �־� ���� ���ӽ����̽��� ������
	int maxChannel; //�ִ� ������ ��
	float distanceFactor; //�Ÿ� ����ġ
	bool bInitialized; //�ʱ�ȭ �Ǻ�

	int Check; //���� üũ
};