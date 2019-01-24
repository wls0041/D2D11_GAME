#pragma once
#include "IResource.h"

enum class RepeatType :uint {Once, Loop};

struct Keyframe { //keyframe�ϳ��ϳ��� �ؽ���, 
	Keyframe() { ZeroMemory(this, sizeof(Keyframe)); }
	Keyframe(const string &filePath, const Vector2 &offset, const Vector2 &size, const float &time)
		: filePath(filePath), offset(offset), size(size), time(time) {}
	string filePath;
	Vector2 offset; //sprite���� �� texture�� ������ġ
	Vector2 size; //offset���� x��, y���� ũ��
	float time; //animation���� �ʿ��� �ӵ��� �����ϱ� ���� time����
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

	void AddKeyframe(const string &filePath, const Vector2 &offset, const Vector2 &size, const float &time);

private:
	RepeatType repeatType;
	vector<Keyframe> keyframes; //animation�� keyframe�� ����
};