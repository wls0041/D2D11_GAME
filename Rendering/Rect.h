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

	ID3D11InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����

	D3DXMATRIX world; //�������

	struct Data { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
		D3DXMATRIX World; //�������
	}data;

	ID3D11Buffer *cbuffer; //hlsl�� VS�� ������ ������ ����

	ID3D11RasterizerState *rsState;

	ID3D11ShaderResourceView *diffuseMap; //diffuse : �ڱⰡ �������� ��, shader�ȿ� texture�� ����ֱ� ���� ��ȯ
	ID3D11ShaderResourceView *diffuseMap2;

	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)
};