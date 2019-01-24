#pragma once
#include "stdafx.h"

//cbuffer에 들어가는 데이터 모음
enum class ShaderType : uint
{
	VS, PS, GS, CS
};

struct CameraData { //데이터를 넘기기 편하게 구조체로 선언
	Matrix View; //시야공간
	Matrix Projection; //투영공간
}data;

struct WorldData { //데이터를 넘기기 편하게 구조체로 선언
	Matrix World; //세계공간
};

struct ColorData {
	Color Color;
};

struct AnimationData {
	Vector2 TextureSize;
	Vector2 SpriteOffset;
	Vector2 Spritesize;
	float Padding[2]; //공간맞추기 24byte + 8byte
};