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

	VertexShader *vertexShader;	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	PixelShader *pixelShader;

	InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class Collider *collider;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	ConstantBuffer *spriteBuffer;

	Vector3 moveDir; //공의 x, y움직임이 양방향, 음방향인지 결정
	float jumpSpeed;
	float jumpAccel;
	bool curCheck_X;
};