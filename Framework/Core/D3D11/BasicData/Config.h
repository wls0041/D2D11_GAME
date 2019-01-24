#pragma once
#include "stdafx.h"

//cbuffer�� ���� ������ ����
enum class ShaderType : uint
{
	VS, PS, GS, CS
};

struct CameraData { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
	Matrix View; //�þ߰���
	Matrix Projection; //��������
}data;

struct WorldData { //�����͸� �ѱ�� ���ϰ� ����ü�� ����
	Matrix World; //�������
};

struct ColorData {
	Color Color;
};

struct AnimationData {
	Vector2 TextureSize;
	Vector2 SpriteOffset;
	Vector2 Spritesize;
	float Padding[2]; //�������߱� 24byte + 8byte
};