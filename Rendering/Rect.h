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

	VertexColor *vertices;
	ID3D11Buffer *vertexBuffer;

	uint *indices;
	ID3D11Buffer *indexBuffer;

	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	//hlsl�� �츮 �����Ϸ��� ���� ����. �׷��� Ȯ���� �ʿ��� �������� �ؾ��ϰ� 
	//������ �� ���� obj������ �������� ������ �� ������ ������ ���� �ʿ���(Blob)
	ID3D10Blob *vsBlob;
	ID3D10Blob *psBlob;

	ID3D11InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����

	D3DXMATRIX world; //�������

	struct Data { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
		D3DXMATRIX World; //�������
	}data;

	ID3D11Buffer *cbuffer; //hlsl�� VS�� ������ ������ ����

	ID3D11RasterizerState *rsState;
};