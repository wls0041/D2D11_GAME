#pragma once

class Texture
{
public:
	Texture(class Context *context);
	virtual ~Texture();

	void CreateTextureFromFile(const string &filePath);
	void CreateTexture2D(const float &width, const float &height, const DXGI_FORMAT &format);
	void Clear();

	void BindPipeline(const ShaderType &type, const uint &slot);

private:
	class Graphics *graphics;
	ID3D11ShaderResourceView *srv;//class�� ��ǥ
	
	DXGI_FORMAT format;
	string filePath;
	float width; //���� ũ��
	float height;
};