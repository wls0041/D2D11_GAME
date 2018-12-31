#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Context * context) : cbuffer(nullptr)
{
	graphics = context->GetSubsystem<Graphics>();
}

ConstantBuffer::~ConstantBuffer()
{
	Clear();
}

void ConstantBuffer::Clear()
{
	SAFE_RELEASE(cbuffer);
}

void * ConstantBuffer::Map()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(SUCCEEDED(hr));

	return mappedResource.pData;
}

void ConstantBuffer::Unmap()
{
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);
}

void ConstantBuffer::BindPipeline(const ShaderType & type, const uint & slot)
{
	auto dc = graphics->GetDeviceContext();
	
	switch (type)
	{
	case ShaderType::VS: dc->VSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::PS: dc->PSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::GS: dc->GSSetConstantBuffers(slot, 1, &cbuffer); break;
	case ShaderType::CS: dc->CSSetConstantBuffers(slot, 1, &cbuffer); break;
	}
}
