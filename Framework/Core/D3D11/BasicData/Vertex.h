#pragma once

struct VertexColor { //기본 생성자를 내포, 생정자를 만들어주면 기본 생성자 삭제
	VertexColor() : position(0, 0, 0), color(0, 0, 0, 1) {}
	VertexColor(const D3DXVECTOR3 &position,  const D3DXCOLOR &color) : position(position), color(color) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;

	static D3D11_INPUT_ELEMENT_DESC  descs[]; //몇번을 만들던 하나만 만들도록 static으로, cpp에서 생성(컴파일러에 전역으로 쓸 것이라고 알려주기 위함)
	static const uint count = 2;
};


struct VertexTexture { //기본 생성자를 내포, 생정자를 만들어주면 기본 생성자 삭제
	VertexTexture() : position(0, 0, 0), uv(0, 0) {}
	VertexTexture(const D3DXVECTOR3 &position, const D3DXVECTOR2 &uv) : position(position), uv(uv) {}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;

	static D3D11_INPUT_ELEMENT_DESC  descs[]; //몇번을 만들던 하나만 만들도록 static으로, cpp에서 생성(컴파일러에 전역으로 쓸 것이라고 알려주기 위함)
	static const uint count = 2;
};