#pragma once
#include "stdafx.h"

//cbuffer�� ���� ������ ����
enum class ShaderType : uint
{
	VS, PS, GS, CS
};

struct CameraData { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
	D3DXMATRIX View; //�þ߰���
	D3DXMATRIX Projection; //��������
}data;

struct WorldData { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
	D3DXMATRIX World; //�������
};

struct ColorData {
	D3DXCOLOR Color;
};

struct AnimationData {
	D3DXVECTOR2 TextureSize;
	D3DXVECTOR2 SpriteOffset;
	D3DXVECTOR2 Spritesize;
	float Padding[2]; //�������߱� 24byte + 8byte
};