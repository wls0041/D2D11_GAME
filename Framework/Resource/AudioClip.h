#pragma once
#include "IResource.h"

enum class PlayType : uint {Sound, Stream};               

class AudioClip : public IResource
{
public:
	AudioClip(class Context *context);
	virtual ~AudioClip();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	void Play();

	void SetPlayType(const PlayType &type) { this->playType = playType; }
	
	const bool IsPlaying();

private:
	void CreateSound(const string &filePath);
	void CreateStream(const string &filePath);

	const int GetSoundMode(); 
	const bool IsChannelValid(); //�������� ��ȿ�Ѱ�

private:
	FMOD::System *system;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	PlayType playType;
	int loopMode;
	int rolloffMode;
	float minDistance;
	float maxDistance; //�Ҹ��� ���� ���� �ƴ�. ���Ұ� �ȵǴ� �Ÿ�
	int check;
};
