#pragma once

class Bullet
{
public:
	Bullet(class Context *context);
	virtual ~Bullet();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }
	const bool &GetExist() const { return bExist; }
	const Vector3 &GetSize();

	void SetExist(const bool &bExist) { this->bExist = bExist; }
	void SetCollider();

	void Update();
	void Render();

private:
	class Context *context;
	class Graphics *graphics;

	Geometry<VertexTexture> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	VertexShader *vertexShader;	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	PixelShader *pixelShader;

	InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class Collider *collider;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	class Animator *animator;
	ConstantBuffer *animationBuffer;
	
	bool bExist;
};