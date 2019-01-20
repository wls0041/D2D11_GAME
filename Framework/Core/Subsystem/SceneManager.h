#pragma once
#include "ISubsystem.h"

class SceneManager : public ISubsystem
{
public:
	SceneManager(class Context *context);
	virtual ~SceneManager();

	void Initialize() override;

	void Update();
	void Render();
	void CheckBest();

	class Scene *GetCurrentScene() const { return currentScene; }
	void SetCurrentScene(const string &name);
	void RegisterScene(const string &name, class Scene *scene);

private:
	class Scene *currentScene; //검색된 씬을 꺼내놓고 그걸 쓰다가 다른 씬이 필요하면 그 때 다시 검색
	map<string, class Scene*> scenes; //씬은 한 번에 하나만 필요하기 때문에 검색이 빠른 map을 사용, map의 key값은 고유함
	int bestScore;
	int curBestScore;
};