#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Context * context) : vbuffer(nullptr), stride(0), offset(0)
{
	graphics = context->GetSubsystem<Graphics>();
}

VertexBuffer::~VertexBuffer()
{
	Clear();
}

void VertexBuffer::Clear()
{
	SAFE_RELEASE(vbuffer);
	stride = 0;
	offset = 0;
}

void VertexBuffer::BindPipeline()
{
	graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset); //정점정보 삽입
}
