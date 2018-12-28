#include "stdafx.h"
#include "VertexShader.h"

VertexShader::VertexShader(Context * context) : BaseShader(context), shader(nullptr), blob(nullptr)
{
}

VertexShader::~VertexShader()
{
	Clear();
}

void VertexShader::Create(const string & filePath, const string & fuctionName)
{
	this->shaderFilePath = filePath;
	this->functionName = fuctionName;
	ID3D10Blob *error = nullptr;

	//Color.hlsl에 들어있는 VS를 vs_5버전으로 컴파일해서 vsBlob에 넣어준다.
	HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*으로 받겠다, FileA - char*로 받겠다
		shaderFilePath.c_str(), //src(source) 원본파일,  dest(destination) 복사파일
		nullptr, nullptr,
		functionName.c_str(), //우리가 만든 함수 이름
		"vs_5_0",  //컴파일 수준
		D3D10_SHADER_ENABLE_STRICTNESS, //엄밀히 체크해 구식문법를 사용금지함 
		0, nullptr,
		&blob,  //byte코드
		&error, nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreateVertexShader(
		blob->GetBufferPointer(), //시작점부터
		blob->GetBufferSize(),    //사이즈 크기까지
		nullptr,
		&shader
	);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(error);
}

void VertexShader::Clear()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(blob);
}

void VertexShader::BindPipeline()
{
	graphics->GetDeviceContext()->VSSetShader(shader, nullptr, 0);
}
