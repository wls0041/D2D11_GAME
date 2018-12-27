#pragma once

class BaseShader
{
public:
	BaseShader(class Context *context);
	virtual ~BaseShader() {}

	virtual void BindPipeline() = 0;

protected:
	const bool CheckShaderError(const HRESULT &hr, ID3D10Blob *error); //messagebox�� shader���� �� �� ������ ���

protected:
	class Graphics *graphics;
	string shaderFilePath;
	string functionName;
};