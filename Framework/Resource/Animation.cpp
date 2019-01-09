#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context * context) : IResource(context), repeatType(RepeatType::Once)
{
}

Animation::~Animation()
{
	keyframes.clear();
	keyframes.shrink_to_fit();
}

void Animation::SaveToFile(const string & filePath)
{
}

void Animation::LoadFromFile(const string & filePath)
{
}

Keyframe * Animation::GetKeyframeFromIndex(const uint & index)
{
	if(keyframes.size() <= index) return nullptr;
	return &keyframes[index];
}

void Animation::AddKeyframe(const string & filePath, const D3DXVECTOR2 & offset, const D3DXVECTOR2 & size, const float & time)
{
	keyframes.emplace_back(filePath, offset, size, time);
}
