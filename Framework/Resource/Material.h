#pragma once
#include "IResource.h"
//mesh : 그물, 골격
//material : 재질
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
	class VertexShader *vertexShader; //매칭위해
	class PixelShader *pixelShader; //매칭위해
	class Texture *diffuseTexture;
	D3DXCOLOR diffuseColor;

	//speculat : 반사광, 반사율을 미리 설정
	//diffuse : 물체에 반사된 색(난반사)
	//normal : 한 점의 수직벡터를 계산해서 그림자를 만듬, 파란 이미지
};