#pragma once

class Ball
{
public:
	Ball(class Context *context);
	virtual ~Ball();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }

	void SetCollider();
	void SetMoveDir(const Vector3 &moveDir) { this->moveDir = moveDir; }
	void SetCurCheck(const bool &curCheck_X) { this->curCheck_X = curCheck_X; }
	void InvMoveDir() { curCheck_X == true ? moveDir.x = -moveDir.x : moveDir.y = -moveDir.y; }

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

	Vector3 moveDir; //���� x, y�������� �����, ���������� ����
	float jumpSpeed;
	float jumpAccel;
	bool curCheck_X;
};