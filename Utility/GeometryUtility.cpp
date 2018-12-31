#include "stdafx.h"
#include "GeometryUtility.h"

void GeometryUtility::CreateQuad(Geometry<VertexColor>& geometry)
{
	geometry.AddVertex(VertexColor(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), D3DXCOLOR(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), D3DXCOLOR(0, 0, 0, 1)));
	geometry.AddVertex(VertexColor(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), D3DXCOLOR(0, 0, 0, 1)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateQuad(Geometry<VertexTexture>& geometry)
{
	geometry.AddVertex(VertexTexture(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0, 1)));
	geometry.AddVertex(VertexTexture(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), D3DXVECTOR2(0, 0)));
	geometry.AddVertex(VertexTexture(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), D3DXVECTOR2(1, 1)));
	geometry.AddVertex(VertexTexture(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), D3DXVECTOR2(1, 0)));

	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2);
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);
}

void GeometryUtility::CreateCircle(Geometry<VertexColor>& geometry)
{
	int size = 36; //원을 그릴 삼각형의 개수
	float angle = 360 / size; //세타

	float r = static_cast<float>(rand() % 10) / static_cast<float>(10);
	float g = static_cast<float>(rand() % 10) / static_cast<float>(10);
	float b = static_cast<float>(rand() % 10) / static_cast<float>(10);

	//cos(0) = 1, sin(0) = 0이므로 따로 빼서 추가
	geometry.AddVertex(VertexColor(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(r,g,b,1)));

	for (int i = 1; i <= size; i++)
		geometry.AddVertex(VertexColor(D3DXVECTOR3(cosf(D3DXToRadian( angle * i)), sinf(D3DXToRadian(angle * i)), 0.0f), D3DXCOLOR(r, g, b, 1)));

	for (int i = 0; i < size; i++)
	{
		geometry.AddIndex(0); //원점

		for (int j = 1; j < 3; j++) {
			if ((i + j) % (size + 1) == 0) geometry.AddIndex(1); //마지막 35-1 중 1인덱스 추가
			else geometry.AddIndex(i + j); //루프를 반복하며 1-2, 2-3, 3-4, ...., 34-35, 35의 인덱스 추가
		}
	}
}
