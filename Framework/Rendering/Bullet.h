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

	VertexShader *vertexShader;	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class Collider *collider;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	class Animator *animator;
	ConstantBuffer *animationBuffer;
	
	bool bExist;
};