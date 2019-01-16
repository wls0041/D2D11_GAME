#pragma once

class AudioSource
{
public:
	AudioSource(class Context *context);
	virtual ~AudioSource();

	void SetAudioClip(const string &filePath);

	void Play();
	void Pause();
	void Stop();

	const int &GetPriority() const { return priority; }
	const bool &IsLoop() const { return bLoop; }
	const bool &IsMute() const { return bMute; }
	const float &GetVolume() const { return volume; }
	const float &GetPitch() const { return pitch; }
	const float &GetPan() const { return pan; }

	void SetPriority(const int &priority);
	void SetIsLoop(const bool &bLoop);
	void SetIsMute(const bool &bMute);
	void SetVolume(const float &volume);
	void SetPitch(const float &pitch);
	void SetPan(const float &pan);
										   
private:
	class Context *context;
	class AudioClip *audioClip;
	string filePath;
	float volume;
	float pitch;
	float pan;
	bool bMute;
	bool bLoop;
	int priority;
};