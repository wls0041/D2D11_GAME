#pragma once
#include "IResource.h"

class Effect : public IResource
{
public:
	Effect(class Context *context);
	virtual ~Effect();

	void SaveToFile(const string &filePath) override;
	void LoadFromFile(const string &filePath) override;

	const bool &IsExist() const { return bExist; }

	void SetIsExist(const bool &bExist) { this->bExist = bExist; }
	void SetPosition(const Vector3 &position);
	void SetScale(const Vector3 &scale);
	void SetTexture(const string &filePath);
	void SetAnimation(const string &filePath);

	void Update();
	void Render();

private:
	class Geometry<VertexTexture> geometry;
	class VertexBuffer *vertexBuffer;
	class IndexBuffer *indexBuffer;
	class InputLayout *inputLayout;
	class VertexShader *vertexShader;
	class PixelShader *pixelShader;
	class Texture *texture;
	class Transform *transform;
	class Animator *animator;
	class ConstantBuffer *spriteBuffer;
	class ConstantBuffer *worldBuffer;

	bool bExist;
};