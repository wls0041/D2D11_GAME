#pragma once

class Rect
{
public:
	Rect(class Graphics *graphics);
	virtual ~Rect();

	void Update();
	void Render();

private:
	class Graphics *graphics;

	VertexTexture *vertices;
	ID3D11Buffer *vertexBuffer;

	uint *indices;
	ID3D11Buffer *indexBuffer;

	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	//hlsl은 우리 컴파일러가 하지 않음. 그러나 확인이 필요해 컴파일을 해야하고 
	//컴파일 후 따로 obj파일을 만들지는 않지만 이 정보를 저장할 곳이 필요함(Blob)
	ID3D10Blob *vsBlob;
	ID3D10Blob *psBlob;

	ID3D11InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함

	D3DXMATRIX world; //세계공간

	struct Data { //데이터를 넘기기 편하게 구조체로 선언
		D3DXMATRIX World; //세계공간
	}data;

	ID3D11Buffer *cbuffer; //hlsl의 VS에 정보를 보내줄 버퍼

	ID3D11RasterizerState *rsState;

	ID3D11ShaderResourceView *diffuseMap; //diffuse : 자기가 내보내는 색, shader안에 texture를 집어넣기 위해 변환
	ID3D11ShaderResourceView *diffuseMap2;

	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)
};