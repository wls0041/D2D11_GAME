#pragma once
#include "ISubsystem.h"

class SceneManager : public ISubsystem
{
public:
	SceneManager(class Context *context);
	virtual ~SceneManager();

	void Initialize() override;
	const int &GetLife() const { return life; }

	void Update();
	void Render();

	class Scene *GetCurrentScene() const { return currentScene; }
	void SetCurrentScene(const string &name);

	void RegisterScene(const string &name, class Scene *scene);

private:
	class Scene *currentScene; //�˻��� ���� �������� �װ� ���ٰ� �ٸ� ���� �ʿ��ϸ� �� �� �ٽ� �˻�
	map<string, class Scene*> scenes; //���� �� ���� �ϳ��� �ʿ��ϱ� ������ �˻��� ���� map�� ���, map�� key���� ������
	class Rect *winMessage;
	class Rect *loseMessage;
	class Rect *title;

	class AudioSource *bgm;
	class AudioSource *loseBgm;
	class AudioSource *winBgm;
	class Camera *camera;
	class ConstantBuffer *cameraBuffer;

	bool bLose;
	bool bWin;
	bool bStart;

	int delayTime;
	int life;
};