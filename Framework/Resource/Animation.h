#pragma once
#include "IResource.h"

enum class RepeatType :uint {Once, Loop};

struct Keyframe { //keyframe하나하나가 텍스쳐, 
	Keyframe() { ZeroMemory(this, sizeof(Keyframe)); }
	Keyframe(const string &filePath, const D3DXVECTOR2 &offset, const D3DXVECTOR2 &size, const float &time)
		: filePath(filePath), offset(offset), size(size), time(time) {}
	string filePath;
	D3DXVECTOR2 offset; //sprite에서 각 texture의 시작위치
	D3DXVECTOR2 size; //offset에서 x축, y축의 크기
	float time; //animation마다 필요한 속도를 조절하기 위한 time변수
};

class Animation : public IResource
{
public:	
	Animation(class Context *context);
	virtual ~Animation();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	Keyframe *GetKeyframeFromIndex(const uint &index);
	const RepeatType &GetReapeatType() const { return repeatType; }
	const vector<Keyframe> &GetKeyframes() const { return keyframes; }
	const uint GetKeyframeCount() const { return keyframes.size(); }
	
	void SetRepeatType(const RepeatType &repeatType) { this->repeatType = repeatType; }

	void AddKeyframe(const string &filePath, const D3DXVECTOR2 &offset, const D3DXVECTOR2 &size, const float &time);

private:
	RepeatType repeatType;
	vector<Keyframe> keyframes; //animation은 keyframe의 집합
};