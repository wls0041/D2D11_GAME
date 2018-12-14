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
	struct VertexColor {
		D3DXVECTOR3 position;
		D3DXCOLOR color;
	};

private:
	VertexColor *vertices;
	ID3D11Buffer *vertexBuffer;

	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	//hlsl은 우리 컴파일러가 하지 않음. 그러나 확인이 필요해 컴파일을 해야하고 
	//컴파일 후 따로 obj파일을 만들지는 않지만 이 정보를 저장할 곳이 필요함(Blob)
	ID3D10Blob *vsBlob;
	ID3D10Blob *psBlob;

	ID3D11InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	
	D3DXMATRIX world; //세계공간
	D3DXMATRIX view; //시야공간
	D3DXMATRIX projection; //투영공간

	struct Data { //데이터를 넘기기 편하게 구조체로 선언
		D3DXMATRIX World; //세계공간
		D3DXMATRIX View; //시야공간
		D3DXMATRIX Projection; //투영공간
	}data;

	ID3D11Buffer *cbuffer; //hlsl의 VS에 정보를 보내줄 버퍼
};