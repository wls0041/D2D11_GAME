#pragma once
#include "BaseShader.h"

class VertexShader : public BaseShader
{
public:
	VertexShader(class Context *context);
	virtual ~VertexShader();

	ID3D10Blob *GetBlob() const { return blob; } //layout에서 시멘틱 체크에 사용

	void Create(const string &filePath, const string &fuctionName = "VS");
	void Clear();

	void BindPipeline() override;

private:
	ID3D11VertexShader *shader;

	//hlsl은 우리 컴파일러가 하지 않음. 그러나 확인이 필요해 컴파일을 해야하고 
	//컴파일 후 따로 obj파일을 만들지는 않지만 이 정보를 저장할 곳이 필요함(Blob)
	ID3D10Blob *blob; //셰이더의 컴파일된 데이터가 들어감
};