#include "stdafx.h"
#include "PixelShader.h"

PixelShader::PixelShader(Context * context) : BaseShader(context), shader(nullptr)
{
}

PixelShader::~PixelShader()
{
	Clear();
}

void PixelShader::Create(const string & filePath, const string & fuctionName)
{
	this->shaderFilePath = filePath;
	this->functionName = fuctionName;
	ID3D10Blob *error = nullptr, *blob = nullptr;

	HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*���� �ްڴ�, FileA - char*�� �ްڴ�
		shaderFilePath.c_str(), //src(source) ��������,  dest(destination) ��������
		nullptr, nullptr,
		functionName.c_str(), //�츮�� ���� �Լ� �̸�
		"ps_5_0",  //������ ����
		D3D10_SHADER_ENABLE_STRICTNESS,
		0, nullptr,
		&blob,
		&error, nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreatePixelShader(
		blob->GetBufferPointer(), //����������
		blob->GetBufferSize(),    //������ ũ�����
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(error);
	SAFE_RELEASE(blob);
}

void PixelShader::Clear()
{
	SAFE_RELEASE(shader);
}

void PixelShader::BindPipeline()
{
	graphics->GetDeviceContext()->PSSetShader(shader, nullptr, 0);
}
