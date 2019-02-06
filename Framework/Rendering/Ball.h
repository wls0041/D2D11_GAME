#pragma once

class Ball
{
public:
	Ball(class Context *context);
	virtual ~Ball();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }
	const int &GetLevel() const { return level; }
	const int &GetNumber() const { return ballNum; }

	void SetCollider();

	void SetMoveDir(const Vector3 &moveDir) { this->moveDir = moveDir; }
	void SetFloorSpeed(const float &floorSpeed) { this->floorSpeed = floorSpeed; }
	void SetJumpSpeed(const float &jumpSpeed) { this->jumpSpeed.y = jumpSpeed; }
	void SetLevel(const int &level) { this->level = level; }
	void SetNumber(const int &ballNum) { this->ballNum = ballNum; }

	void InvMoveDir(const CircleCheck &check, class Collider *opponent);


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
	Vector3 position;
	Vector3 jumpSpeed;
	float jumpAccel;

	class Collider *opponent;
	CircleCheck bCheck;
	float floorSpeed;
	bool bUpdate;
	int ballNum;
	int time;
	int level;
};