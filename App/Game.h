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

	void IsCollision();

private:
	class Graphics *graphics;
	class Player *player;
	class Rect *rect[10];

	float comScale[10];
	float userScale;
	D3DXVECTOR2 comPosition[10];
	D3DXVECTOR2 userPosition;

	bool collResult[10];

	D3DXMATRIX view; //시야공간
	D3DXMATRIX projection; //투영공간

	struct Data { //데이터를 넘기기 편하게 구조체로 선언
		D3DXMATRIX View; //시야공간
		D3DXMATRIX Projection; //투영공간
	}data;

	ID3D11Buffer *cbuffer; //hlsl의 VS에 정보를 보내줄 버퍼
};