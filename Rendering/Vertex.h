#pragma once

struct VertexColor { //�⺻ �����ڸ� ����, �����ڸ� ������ָ� �⺻ ������ ����
	VertexColor() : position(0, 0, 0), color(0, 0, 0, 1) {}
	VertexColor(const D3DXVECTOR3 &position,  const D3DXCOLOR &color) : position(position), color(color) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;

	static D3D11_INPUT_ELEMENT_DESC  descs[]; //����� ����� �ϳ��� ���鵵�� static����, cpp���� ����(�����Ϸ��� �������� �� ���̶�� �˷��ֱ� ����)
	static const uint count = 2;
};


struct VertexTexture { //�⺻ �����ڸ� ����, �����ڸ� ������ָ� �⺻ ������ ����
	VertexTexture() : position(0, 0, 0), uv(0, 0) {}
	VertexTexture(const D3DXVECTOR3 &position, const D3DXVECTOR2 &uv) : position(position), uv(uv) {}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;

	static D3D11_INPUT_ELEMENT_DESC  descs[]; //����� ����� �ϳ��� ���鵵�� static����, cpp���� ����(�����Ϸ��� �������� �� ���̶�� �˷��ֱ� ����)
	static const uint count = 2;
};