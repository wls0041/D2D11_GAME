#include "stdafx.h"
#include "Vertex.h"

D3D11_INPUT_ELEMENT_DESC VertexColor::descs[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, //format�� ������ DXGI + FLOAT��(32bit)
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} //append~element ���� ���ڸ� ���� �ʾƵ� �˾Ƽ� �������� �����	
};