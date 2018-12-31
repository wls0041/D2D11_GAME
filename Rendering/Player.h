#pragma once

class Player
{
public:
	Player(class Context *context);
	virtual ~Player();

	void Update();
	void Render();
	
	void MoveCircle(D3DXVECTOR2 dir);
	bool IsCollisionEdge(int dir);

	void RaiseScale(float scale);

	const D3DXVECTOR2& GetPosition() const { return curPosition; }
	const float& GetScale() const { return curScale; }

private:
	enum DIRECTION {LT = 1, TT, RT, RR, RB, BB, LB, LL};
	
	class Graphics *graphics;

	Geometry<VertexColor> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

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

	float curScale;
	D3DXVECTOR2 curPosition;

	ID3D11Buffer *cbuffer; //hlsl�� VS�� ������ ������ ����

	ID3D11RasterizerState *rsState;
};