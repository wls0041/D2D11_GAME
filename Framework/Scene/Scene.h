#pragma once

class Scene
{
public:
	Scene(class Context *context);
	virtual ~Scene();

	const string &GetName() const { return name; }
	void SetName(const string &name) { this->name = name; }
	void SetBest(const int &best) { this->bestScore = best; }
	void IsBestPlay(const bool &best) { this->bBestPlay = best; }

	void Update();
	void Render();

	void IsBoardOutBack(D3DXVECTOR3 position, int index);
	void IsBoardOutPipe(D3DXVECTOR3 position, int index);
	void AddScore(float position);

	const bool &GetRestart() const { return bRestart; }
	const int &GetScore() const { return score; }

	void GameEnd();
	void RenderPlayScore(bool bBest, float width = 30.0f, float height = 50.0f, float x = 0.0f, float y = 0.0f);
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
	bool bRestart;
	bool bBestPlay;
	int score;
	int bestScore;
	string name;

	class Camera *camera;
	class ConstantBuffer *cameraBuffer;
	class Input *input;
	class Rect *bird;
	class Anim *board;
	class Anim *back[2];
	class Anim *ground[2];
	class Anim *message[4];
	class Anim *pipe_top[5];
	class Anim *pipe_bot[5];
	class Anim *number[6];

	D3DXVECTOR2 offset[10] = { {288,100},{291,118}, {289,134}, {289,150}, {287,173},
							  {287,185}, {165,245}, {175,245}, {185,245}, {195,245} };

	D3DXVECTOR2 numSize[10] = { { 7, 10 }, { 5, 10 }, { 7, 10 }, { 7, 10 }, { 7, 10 }, 
							  { 7, 10 }, { 7, 10 }, { 7, 10 }, { 7, 10 }, { 7, 10 } };

	vector<class AudioClip*> clips;

	D3DXVECTOR2 backPosition[2];
	D3DXVECTOR2 pipePosition_top[5];
	D3DXVECTOR2 pipePosition_bot[5];
	D3DXVECTOR2 birdPosition;
	D3DXVECTOR2 boardScale;
	D3DXVECTOR2 pipeScale;
	D3DXVECTOR2 numberScale;
};