#include "stdafx.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Context * context) : ibuffer(nullptr)
{
	graphics = context->GetSubsystem<Graphics>();
}

IndexBuffer::~IndexBuffer()
{
	Clear();
}

void IndexBuffer::Create(const vector<uint>& indices, const D3D11_USAGE & usage)
{
	assert(!indices.empty()); //비어있으면 true반환

	Clear();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	if (usage == D3D11_USAGE_DYNAMIC)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(uint) * indices.size();

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = indices.data();

	HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &subData, &ibuffer);
	assert(SUCCEEDED(hr));
}

void IndexBuffer::Clear()
{
	SAFE_RELEASE(ibuffer);
}

void IndexBuffer::BindPipeline()
{
	graphics->GetDeviceContext()->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0); //4byte uint하나를 쓰므로 r32_uint
}
