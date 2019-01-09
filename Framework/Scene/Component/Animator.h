#pragma once

enum class AnimationMode : uint {Play, Pause, Stop};

class Animator
{
public:
	Animator(class Context *context);
	virtual ~Animator();

	void Update();

	class Animation *GetCurrentAnimation() const { return curAnimation; }
	struct Keyframe *GetCurrentkeyframe();

	void SetCurrentAnimation(const string &animationName);
	
	void RegisterAnimation(const string &filePath);
	void RegisterAnimation(class Animation *animation);

	void Play();
	void Stop();
	void Pause();

	const bool IsPlay() const { return mode == AnimationMode::Play ? true : false; }
	const bool IsStop() const { return mode == AnimationMode::Stop ? true : false; }
	const bool IsPause() const { return mode == AnimationMode::Pause ? true : false; }

private:
	class Context *context;
	class Timer *timer;
	class ResourceManager *resourceMgr;

	AnimationMode mode;
	uint curFrameNumber;
	float frameTimer;

	class Animation *curAnimation;
	map<string, class Animation*> animations; //animation�� iresource�� �����ͷ� ��� �ް� �ֱ� ������ �̿� ���߱� ����
	//�ſ� ���� ������ �ʿ��� ������ ����� ��(stack���� hip������ Ŀ��) 
	//�ʿ��� ������ �� ��
};