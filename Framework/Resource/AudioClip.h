#pragma once
#include "IResource.h"

enum class PlayType : uint { Sound, Stream };
//원본. 해결하기 위해 이를 복사해서 나눠줄 예정
//channel과 sound가 하나뿐이므로 하나의 sound를 play하면 그 sound가 끝나기 전까지 다시 sound 재생 불가함
class AudioClip : public IResource
{
public:
	AudioClip(class Context *context);
	AudioClip(const AudioClip &rhs); //복사생성자
	virtual ~AudioClip();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	void Play();
	void Stop();
	void Pause();

	void SetLoop(const bool &bLoop);
	void SetMute(const bool &bMute);
	void SetPriority(const int &priority); //우선순위
	void SetVolume(const float &volume);
	void SetPitch(const float &pitch); //속도로 음낮이 조절
	void SetPan(const float &pan); //좌,우 음향 비중 조절

	void SetPlayType(const PlayType &type) { this->playType = playType; }

	const bool IsPlaying();

private:
	void CreateSound(const string &filePath);
	void CreateStream(const string &filePath);

	const int GetSoundMode();
	const bool IsChannelValid(); //재생목록이 유효한가

private:
	FMOD::System *system; //subsystem에서 관리해주므로 1만 있어도 됨. 깊은 복사 필요 x
	FMOD::Sound *sound; //깊은 복사 필요
	FMOD::Channel *channel;

	PlayType playType;
	int loopMode;
	int rolloffMode;
	float minDistance;
	float maxDistance; //소리를 끄는 것이 아님. 감소가 안되는 거리
	int check;
};
