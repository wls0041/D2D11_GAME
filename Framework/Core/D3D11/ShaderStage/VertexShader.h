#pragma once
#include "BaseShader.h"

class VertexShader : public BaseShader
{
public:
	VertexShader(class Context *context);
	virtual ~VertexShader();

	ID3D10Blob *GetBlob() const { return blob; } //layout���� �ø�ƽ üũ�� ���

	void Create(const string &filePath, const string &fuctionName = "VS");
	void Clear();

	void BindPipeline() override;

private:
	ID3D11VertexShader *shader;

	//hlsl�� �츮 �����Ϸ��� ���� ����. �׷��� Ȯ���� �ʿ��� �������� �ؾ��ϰ� 
	//������ �� ���� obj������ �������� ������ �� ������ ������ ���� �ʿ���(Blob)
	ID3D10Blob *blob; //���̴��� �����ϵ� �����Ͱ� ��
};