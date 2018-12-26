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
