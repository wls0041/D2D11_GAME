#pragma once

class Player
{
public:
	Player(class Context *context);
	virtual ~Player();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }
	const int &GetLife() const { return life; }

	Vector3 GetSize();
	void SetCollider();
	void LoseLife();

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

	float x_clamp;
	int life;
};