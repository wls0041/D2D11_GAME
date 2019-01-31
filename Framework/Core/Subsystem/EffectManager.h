#pragma once
#include "ISubsystem.h"

class EffectManager : public ISubsystem
{
public:
	EffectManager(class Context *context);
	virtual ~EffectManager();

	void Initialize() override;

	void AddEffect(
		const string &textureFilePath,
		const string &animationFilePath,
		const Vector3 &position,
		const Vector3 &scale = Vector3(1.0f)
	);

	void Update();
	void Render();

private:
	vector<class Effect*> effects;
};