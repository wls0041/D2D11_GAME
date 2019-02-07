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
	SAFE_DELETE(audioClip); //�̹� ���õ� �� ������ �����Ƿ� ��������

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
	//ä�� �켱����
	// 0(�ּ�), 256(����), 128(�߰�)

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

	this->volume = Math::clamp(volume, 0.0f, 1.0f); //0~1�� ����ȭ �Ǿ� ����
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

	this->pan = Math::clamp(pitch,-1.0f, 1.0f); //��Ʈ���� ����ī�尡 �ϳ��� �����־� �ٲ㵵 �Ȱ���
	audioClip->SetPan(this->pan);
}
