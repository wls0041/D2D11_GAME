#pragma once
#include "stdafx.h"

//cbuffer에 들어가는 데이터 모음
enum class ShaderType : uint
{
	VS, PS, GS, CS
};

struct CameraData { //데이터를 넘기기 편하게 구조체로 선언
	D3DXMATRIX View; //시야공간
	D3DXMATRIX Projection; //투영공간
}data;

struct WorldData { //데이터를 넘기기 편하게 구조체로 선언
	D3DXMATRIX World; //세계공간
};

struct ColorData {
	D3DXCOLOR Color;
};

struct AnimationData {
	D3DXVECTOR2 TextureSize;
	D3DXVECTOR2 SpriteOffset;
	D3DXVECTOR2 Spritesize;
	float Padding[2]; //공간맞추기 24byte + 8byte
};