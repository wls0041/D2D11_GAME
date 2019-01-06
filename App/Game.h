#pragma once
#include "App.h"

//pipeline����
class Game final : public App
{
//�θ𿡼� Run���� �������� �ڵ带 ��� ���� �ʿ��� �͸� ���ָ� �˾Ƽ� ��
public:
	Game() : GameStart(false) {}
	~Game(){}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void IsBoardOut(D3DXVECTOR3 position, int index);
	void IsCollision_CircleRect();
	bool IsCollision_CirclePoint(float point_X, float point_Y);

private:
	class Graphics *graphics;
	class Timer *timer;
	class Input *input;
	class Rect *rect[6];
	class Player *player;
	class ConstantBuffer *cameraBuffer; 

	D3DXMATRIX view; //�þ߰���
	D3DXMATRIX projection; //��������

	bool GameStart;
};