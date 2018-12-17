#include "stdafx.h"
#include "Vertex.h"

D3D11_INPUT_ELEMENT_DESC VertexColor::descs[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, //format이 붙으면 DXGI + FLOAT형(32bit)
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0} //append~element 굳이 숫자를 넣지 않아도 알아서 시작점을 잡아줌	
};