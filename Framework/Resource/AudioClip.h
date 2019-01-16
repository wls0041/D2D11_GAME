#pragma once
#include "IResource.h"

enum class PlayType : uint { Sound, Stream };
//����. �ذ��ϱ� ���� �̸� �����ؼ� ������ ����
//channel�� sound�� �ϳ����̹Ƿ� �ϳ��� sound�� play�ϸ� �� sound�� ������ ������ �ٽ� sound ��� �Ұ���
class AudioClip : public IResource
{
public:
	AudioClip(class Context *context);
	AudioClip(const AudioClip &rhs); //���������
	virtual ~AudioClip();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	void Play();
	void Stop();
	void Pause();

	void SetLoop(const bool &bLoop);
	void SetMute(const bool &bMute);
	void SetPriority(const int &priority); //�켱����
	void SetVolume(const float &volume);
	void SetPitch(const float &pitch); //�ӵ��� ������ ����
	void SetPan(const float &pan); //��,�� ���� ���� ����

	void SetPlayType(const PlayType &type) { this->playType = playType; }

	const bool IsPlaying();

private:
	void CreateSound(const string &filePath);
	void CreateStream(const string &filePath);

	const int GetSoundMode();
	const bool IsChannelValid(); //�������� ��ȿ�Ѱ�

private:
	FMOD::System *system; //subsystem���� �������ֹǷ� 1�� �־ ��. ���� ���� �ʿ� x
	FMOD::Sound *sound; //���� ���� �ʿ�
	FMOD::Channel *channel;

	PlayType playType;
	int loopMode;
	int rolloffMode;
	float minDistance;
	float maxDistance; //�Ҹ��� ���� ���� �ƴ�. ���Ұ� �ȵǴ� �Ÿ�
	int check;
};
