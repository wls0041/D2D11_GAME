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