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
	class Rect *rect;

	D3DXMATRIX view; //�þ߰���
	D3DXMATRIX projection; //��������

	struct Data { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
		D3DXMATRIX View; //�þ߰���
		D3DXMATRIX Projection; //��������
	}data;

	ID3D11Buffer *cbuffer; //hlsl�� VS�� ������ ������ ����
};