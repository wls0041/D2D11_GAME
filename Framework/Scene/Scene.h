#pragma once

class Scene
{
public:
	Scene(class Context *context);
	virtual ~Scene();

	const string &GetName() const { return name; }
	void SetName(const string &name) { this->name = name; }

	void Update();
	void Render();

private:
	class Context *context;
	string name;

	class Camera *camera;
	class ConstantBuffer *cameraBuffer;
	class Player *player;

	class AudioSource *bgm;
	vector<class AudioSource*> sources;
};