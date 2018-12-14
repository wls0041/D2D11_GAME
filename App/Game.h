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
	struct VertexColor {
		D3DXVECTOR3 position;
		D3DXCOLOR color;
	};

private:
	VertexColor *vertices;
	ID3D11Buffer *vertexBuffer;

	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	//hlsl�� �츮 �����Ϸ��� ���� ����. �׷��� Ȯ���� �ʿ��� �������� �ؾ��ϰ� 
	//������ �� ���� obj������ �������� ������ �� ������ ������ ���� �ʿ���(Blob)
	ID3D10Blob *vsBlob;
	ID3D10Blob *psBlob;

	ID3D11InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	
	D3DXMATRIX world; //�������
	D3DXMATRIX view; //�þ߰���
	D3DXMATRIX projection; //��������

	struct Data { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
		D3DXMATRIX World; //�������
		D3DXMATRIX View; //�þ߰���
		D3DXMATRIX Projection; //��������
	}data;

	ID3D11Buffer *cbuffer; //hlsl�� VS�� ������ ������ ����
};