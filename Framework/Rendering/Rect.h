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

	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	VertexShader *vertexShader;
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;

	D3DXMATRIX world; //�������

	ID3D11RasterizerState *rsState;

	ID3D11ShaderResourceView *diffuseMap; //diffuse : �ڱⰡ �������� ��, shader�ȿ� texture�� ����ֱ� ���� ��ȯ
	ID3D11ShaderResourceView *diffuseMap2;

	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)
};