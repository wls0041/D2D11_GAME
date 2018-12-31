#pragma once

class ConstantBuffer//hlsl의 VS에 정보를 보내줄 버퍼
{
public:
	ConstantBuffer(class Context *context);
	virtual ~ConstantBuffer();

	template <typename T>
	void Create(const D3D11_USAGE &usage = D3D11_USAGE_DYNAMIC);
	void Clear();
	
	void *Map();
	void Unmap();

	void BindPipeline(const ShaderType &type, const uint &slot);

private:
	class Graphics *graphics;
	ID3D11Buffer *cbuffer;
};

template<typename T>
inline void ConstantBuffer::Create(const D3D11_USAGE & usage)
{
	Clear();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	if (usage == D3D11_USAGE_DYNAMIC) desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(T);

	HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, nullptr, &cbuffer);
	assert(SUCCEEDED(hr));
}
