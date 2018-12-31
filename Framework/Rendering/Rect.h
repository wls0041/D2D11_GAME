#pragma once

class Rect
{
public:
	Rect(class Context *context);
	virtual ~Rect();

	void Update();
	void Render();

private:
	class Graphics *graphics;

	Geometry<VertexTexture> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	VertexShader *vertexShader;
	PixelShader *pixelShader;

	InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	ConstantBuffer *worldBuffer;

	D3DXMATRIX world; //세계공간

	ID3D11RasterizerState *rsState;

	ID3D11ShaderResourceView *diffuseMap; //diffuse : 자기가 내보내는 색, shader안에 texture를 집어넣기 위해 변환
	ID3D11ShaderResourceView *diffuseMap2;

	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)
};