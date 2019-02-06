#pragma once

enum class AnimationMode : uint { Play, Pause, Stop };

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

	void SaveToFile(const string &filePath);
	void LoadFromFile(const string &filePath);

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
	bool isEnd; //Once타입의 애니메이션을 1회 돌렸나 확인

	class Animation *curAnimation;
	map<string, class Animation*> animations; //animation이 iresource를 포인터로 상속 받고 있기 때문에 이에 맞추기 위함
	//매우 많은 공간이 필요할 것으로 예상될 때(stack보다 hip공간이 커서) 
	//필요한 공간을 모를 때
};