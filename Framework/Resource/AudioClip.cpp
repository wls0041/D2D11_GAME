#include "stdafx.h"
#include "AudioClip.h"

AudioClip::AudioClip(Context * context) : IResource(context), sound(nullptr), channel(nullptr), playType(PlayType::Sound), minDistance(1.0f), 
										  maxDistance(1000.0f), loopMode(FMOD_LOOP_OFF), rolloffMode(FMOD_3D_LINEARROLLOFF), check(FMOD_OK)
{
	system = context->GetSubsystem<Audio>()->GetFMODSystem();

}

AudioClip::~AudioClip()
{
	if (!sound) return;
	check = sound->release();
	assert(check == FMOD_OK);
}

void AudioClip::SaveToFile(const string & filePath)
{
}

void AudioClip::LoadFromFile(const string & filePath)
{
	switch (playType) {
	case PlayType::Sound:  CreateSound(filePath); break;
	case PlayType::Stream: CreateStream(filePath); break;
	}
}

void AudioClip::Play()
{
	//if (IsChannelValid()) if (IsPlaying()) return;

	//check = system->playSound(sound, nullptr, false, &channel);
	//assert(check == FMOD_OK);
}

const bool AudioClip::IsPlaying()
{
	if(!IsChannelValid()) return false;
	
	bool bPlaying = true;
	check = channel->isPlaying(&bPlaying);
	assert(check == FMOD_OK);
	
	return bPlaying;
}

void AudioClip::CreateSound(const string & filePath)
{
	check = system->createSound(filePath.c_str(), GetSoundMode(), nullptr, &sound);
	assert(check == FMOD_OK);

	check = sound->set3DMinMaxDistance(minDistance, maxDistance);
	assert(check == FMOD_OK);
}

void AudioClip::CreateStream(const string & filePath)
{
	check = system->createStream(filePath.c_str(), GetSoundMode(), nullptr, &sound);
	assert(check == FMOD_OK);

	check = sound->set3DMinMaxDistance(minDistance, maxDistance);
	assert(check == FMOD_OK);
}

const int AudioClip::GetSoundMode()
{
	return FMOD_3D | loopMode | rolloffMode;
}

const bool AudioClip::IsChannelValid()
{
	if (!channel) return false;
	return true;
}
