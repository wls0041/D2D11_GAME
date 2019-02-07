#include "stdafx.h"
#include "AudioSource.h"

AudioSource::AudioSource(Context * context) : context(context), audioClip(nullptr), filePath(""), bLoop(false), bMute(false), priority(128), volume(1.0f), pitch(1.0f), pan(0.0f)
{
}

AudioSource::~AudioSource()
{
	SAFE_DELETE(audioClip);
}

void AudioSource::SetAudioClip(const string & filePath)
{
	SAFE_DELETE(audioClip); //이미 세팅된 놈 있으면 터지므로 지워버림

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>(filePath);
	
	audioClip = new AudioClip(*clip);
}

void AudioSource::Play()
{
	if (!audioClip) return;
	audioClip->Play();
}

void AudioSource::Pause()
{
	if (!audioClip) return;
	audioClip->Pause();
}

void AudioSource::Stop()
{
	if (!audioClip) return;
	audioClip->Stop();
}

void AudioSource::SetPriority(const int & priority)
{
	//채널 우선순위
	// 0(최선), 256(최하), 128(중간)

	if (!audioClip) return;
	if (this->priority == priority) return;

	this->priority = Math::clamp(priority, 0, 256);
	audioClip->SetPriority(this->priority);
}

void AudioSource::SetIsLoop(const bool & bLoop)
{
	if (!audioClip) return;
	if (this->bLoop == bLoop) return;

	this->bLoop = bLoop;
	audioClip->SetLoop(this->bLoop);
}

void AudioSource::SetIsMute(const bool & bMute)
{
	if (!audioClip) return;
	if (this->bMute == bMute) return;

	this->bMute = bMute;
	audioClip->SetMute(this->bMute);
}

void AudioSource::SetVolume(const float & volume)
{
	if (!audioClip) return;
	//if (this->volume == volume) return;

	this->volume = Math::clamp(volume, 0.0f, 1.0f); //0~1로 정규화 되어 있음
	audioClip->SetVolume(this->volume);
}

void AudioSource::SetPitch(const float & pitch)
{
	if (!audioClip) return;
	if (this->pitch == pitch) return;

	this->pitch = Math::clamp(pitch, 0.0f, 3.0f);
	audioClip->SetPitch(this->pitch);
}

void AudioSource::SetPan(const float & pan)
{
	if (!audioClip) return;
	if (this->pan == pan) return;

	this->pan = Math::clamp(pitch,-1.0f, 1.0f); //노트북은 사운드카드가 하나로 잡혀있어 바꿔도 똑같음
	audioClip->SetPan(this->pan);
}
