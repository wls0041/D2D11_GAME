#pragma once

class Back
{
public:
	Back(class Context *context);
	virtual ~Back();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }

	void SetCollider();
	void SetOffset(const Vector2 &offset) { this->offset = offset; }

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

	ConstantBuffer *spriteBuffer;

	Vector2 offset;
};