#pragma once
#include "IResource.h"

class Texture : public IResource
{
public:
	Texture(class Context *context);
	virtual ~Texture();
	
	void SaveToFile(const string &filePath);
	void LoadFromFile(const string &filePath);

	void CreateTextureFromFile(const string &filePath);
	void CreateTexture2D(const float &width, const float &height, const DXGI_FORMAT &format);
	void Clear();

	void BindPipeline(const ShaderType &type, const uint &slot);

private:
	class Graphics *graphics;
	ID3D11ShaderResourceView *srv;//class의 목표
	
	DXGI_FORMAT format;
	string filePath;
	float width; //원본 크기
	float height;
};