#pragma once
#include "App.h"

//pipeline세팅
class Game final : public App
{
//부모에서 Run등의 지저분한 코드를 모두 가져 필요한 것만 써주면 알아서 돔
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

	D3DXMATRIX view; //시야공간
	D3DXMATRIX projection; //투영공간
};