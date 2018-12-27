#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader(Context * context) : shaderFilePath(""), functionName("")
{
	graphics = context->GetSubsystem<Graphics>();
}

const bool BaseShader::CheckShaderError(const HRESULT & hr, ID3D10Blob * error)
{
	//error�� ���ڿ��� ����ǳ� blob�� �������̽��̱� ������ �ٷ� ���ڿ��� ������ �Ұ�
	if (FAILED(hr)) {
		if (error != nullptr) {
			string str = reinterpret_cast<const char*>(error->GetBufferPointer()); //���ڿ��� ����
			MessageBoxA(nullptr, str.c_str(), "SHADER ERROR!!", MB_OK);
		}
		return false;
	}

	return true;
}
