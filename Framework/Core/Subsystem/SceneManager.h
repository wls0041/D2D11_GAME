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
	class Scene *currentScene; //�˻��� ���� �������� �װ� ���ٰ� �ٸ� ���� �ʿ��ϸ� �� �� �ٽ� �˻�
	map<string, class Scene*> scenes; //���� �� ���� �ϳ��� �ʿ��ϱ� ������ �˻��� ���� map�� ���, map�� key���� ������
	int bestScore;
	int curBestScore;
};