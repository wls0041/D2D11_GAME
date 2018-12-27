#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader(Context * context) : shaderFilePath(""), functionName("")
{
	graphics = context->GetSubsystem<Graphics>();
}

const bool BaseShader::CheckShaderError(const HRESULT & hr, ID3D10Blob * error)
{
	//error시 문자열이 저장되나 blob은 인터페이스이기 때문에 바로 문자열에 저장이 불가
	if (FAILED(hr)) {
		if (error != nullptr) {
			string str = reinterpret_cast<const char*>(error->GetBufferPointer()); //문자열의 시작
			MessageBoxA(nullptr, str.c_str(), "SHADER ERROR!!", MB_OK);
		}
		return false;
	}

	return true;
}
