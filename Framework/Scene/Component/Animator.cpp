#include "stdafx.h"
#include "Animator.h"

Animator::Animator(Context * context) : context(context), mode(AnimationMode::Play), curFrameNumber(0), frameTimer(0.0f), curAnimation(nullptr)
{
	timer = context->GetSubsystem<Timer>();
	resourceMgr = context->GetSubsystem<ResourceManager>();
}

Animator::~Animator()
{
}

void Animator::Update()
{
}

Keyframe * Animator::GetCurrentkeyframe()
{
	if (!curAnimation) return nullptr;
	return curAnimation->GetKeyframeFromIndex(curFrameNumber);
}

void Animator::SetCurrentAnimation(const string & animationName)
{
	assert(animations.count(animationName) > 0);
	
	curFrameNumber = 0;
	frameTimer = 0.0f;
	curAnimation = animations[animationName];
}

void Animator::RegisterAnimation(const string & filePath)
{
	RegisterAnimation(resourceMgr->Load<Animation>(filePath));
}

void Animator::RegisterAnimation(Animation * animation)
{
	assert(animation); //animation�� null�� ���
	assert(animations.count(animation->GetResourceName()) < 1);//�̹� ���� ���

	animations[animation->GetResourceName()] = animation;
}

void Animator::Play()
{
}

void Animator::Stop()
{
}

void Animator::Pause()
{
}
