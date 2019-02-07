#pragma once

class Scene
{
public:
	Scene(class Context *context, const int &life);
	virtual ~Scene();

	const string &GetName() const { return name; }
	const bool &IsDead() const { return bDeadFinish; }
	const bool &IsClear() const { return bClear; }
	const int &GetLife() const { return life; }

	void SetLife(const int &life) { this->life = life; }
	void SetName(const string &name) { this->name = name; }

	void Update();
	void Render();

private:
	class Context *context;
	string name;

	class Camera *camera;
	class ConstantBuffer *cameraBuffer;
	class BallManager *ballManager;
	class Rect *message;
	class Player *player;
	class Back *back;
	class Block *block;

	class AudioSource *bgm;
	vector<class AudioSource*> sources;

	bool bDeadFinish;
	bool bClear;
	bool bStart;
	int life;
	int time;
	int bLive;
};