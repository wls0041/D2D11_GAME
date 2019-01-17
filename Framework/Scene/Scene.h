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
	void GetScore(float position);

	void GameEnd();
	void RenderPlayScore(float width = 30.0f, float height = 50.0f, float x = 0.0f, float y = 0.0f);
	bool IsCollision(D3DXVECTOR2 first, D3DXVECTOR3 second);

private:
	class Context *context;

	float jumpTimer;
	float hitTimer;
	float titleTimer;
	float moveTitle;
	bool GameStart;
	bool bReady;
	bool bJump;
	bool bEnd;
	int score;
	string name;

	class Camera *camera;
	class ConstantBuffer *cameraBuffer;
	class Input *input;
	class Rect *bird;
	class Anim *board;
	class Anim *back[2];
	class Anim *ground[2];
	class Anim *message[3];
	class Anim *pipe_top[5];
	class Anim *pipe_bot[5];
	class Anim *number[10];

	vector<class AudioClip*> clips;

	D3DXVECTOR2 backPosition[2];
	D3DXVECTOR2 pipePosition_top[5];
	D3DXVECTOR2 pipePosition_bot[5];
	D3DXVECTOR2 birdPosition;
	D3DXVECTOR2 boardScale;
	D3DXVECTOR2 pipeScale;
	D3DXVECTOR2 numberScale;
};