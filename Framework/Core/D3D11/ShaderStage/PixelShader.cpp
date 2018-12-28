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

	HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*으로 받겠다, FileA - char*로 받겠다
		shaderFilePath.c_str(), //src(source) 원본파일,  dest(destination) 복사파일
		nullptr, nullptr,
		functionName.c_str(), //우리가 만든 함수 이름
		"ps_5_0",  //컴파일 수준
		D3D10_SHADER_ENABLE_STRICTNESS,
		0, nullptr,
		&blob,
		&error, nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreatePixelShader(
		blob->GetBufferPointer(), //시작점부터
		blob->GetBufferSize(),    //사이즈 크기까지
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
