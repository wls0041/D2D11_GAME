#pragma once
#include "IResource.h"
//mesh : �׹�, ���
//material : ����
class Material : public IResource
{
public:
	Material(class Context * context);
	virtual ~Material();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	class VertexShader *GetVertexShader() const { return vertexShader; }
	class PixelShader *GetPixelShader() const { return pixelShader; }
	class Texture *GetDiffuseTexture() const { return diffuseTexture; }
	const D3DXCOLOR &GetDiffuseColor() const { return diffuseColor; }

	void SetShader(const string &filePath);
	void SetVertexShader(class VertexShader *vertexShader) { this->vertexShader = vertexShader; }
	void SetPixelShader(class PixelShader *pixelShader) { this->pixelShader = pixelShader; }
	void SetDiffuseTexture(class Texture *diffuseTexture) { this->diffuseTexture = diffuseTexture; }
	void SetDiffuseColor(const D3DXCOLOR &diffuseColor) { this->diffuseColor = diffuseColor; }

private:
	class VertexShader *vertexShader; //��Ī����
	class PixelShader *pixelShader; //��Ī����
	class Texture *diffuseTexture;
	D3DXCOLOR diffuseColor;

	//speculat : �ݻ籤, �ݻ����� �̸� ����
	//diffuse : ��ü�� �ݻ�� ��(���ݻ�)
	//normal : �� ���� �������͸� ����ؼ� �׸��ڸ� ����, �Ķ� �̹���
};