#pragma once
#include "App.h"

//pipeline����
class Game final : public App
{
//�θ𿡼� Run���� �������� �ڵ带 ��� ���� �ʿ��� �͸� ���ָ� �˾Ƽ� ��
public:
	Game(){}
	~Game(){}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

private:
	class Graphics *graphics;
	class Timer *timer;
	class Input *input;
	class Rect *rect;
	class ConstantBuffer *cameraBuffer; 

	D3DXMATRIX view; //�þ߰���
	D3DXMATRIX projection; //��������
};