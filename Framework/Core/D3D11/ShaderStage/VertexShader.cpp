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

	//Color.hlsl�� ����ִ� VS�� vs_5�������� �������ؼ� vsBlob�� �־��ش�.
	HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*���� �ްڴ�, FileA - char*�� �ްڴ�
		shaderFilePath.c_str(), //src(source) ��������,  dest(destination) ��������
		nullptr, nullptr,
		functionName.c_str(), //�츮�� ���� �Լ� �̸�
		"vs_5_0",  //������ ����
		D3D10_SHADER_ENABLE_STRICTNESS, //������ üũ�� ���Ĺ����� �������� 
		0, nullptr,
		&blob,  //byte�ڵ�
		&error, nullptr
	);
	assert(CheckShaderError(hr, error));

	hr = graphics->GetDevice()->CreateVertexShader(
		blob->GetBufferPointer(), //����������
		blob->GetBufferSize(),    //������ ũ�����
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
