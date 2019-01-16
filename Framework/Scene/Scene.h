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

	void IsBoardOutBack(D3DXVECTOR3 position, int index);
	void IsBoardOutPipe(D3DXVECTOR3 position, int index);

	void GameEnd();

	bool IsCollision(D3DXVECTOR2 first, D3DXVECTOR3 second, D3DXVECTOR2 scale);

private:
	class Context *context;
	string name;
	bool GameStart;
	bool bJump;
	float jumpTimer;

	class Camera *camera;
	class ConstantBuffer *cameraBuffer;
	class Input *input;
	class Rect *bird;
	class Anim *back[2];
	class Anim *ground[2];
	class Anim *pipe_top[5];
	class Anim *pipe_bot[5];

	vector<class AudioClip*> clips;

	D3DXVECTOR2 backPosition[2];
	D3DXVECTOR2 pipePosition_top[5];
	D3DXVECTOR2 pipePosition_bot[5];
	D3DXVECTOR2 birdPosition;
	D3DXVECTOR2 boardScale;
	D3DXVECTOR2 pipeScale;
};