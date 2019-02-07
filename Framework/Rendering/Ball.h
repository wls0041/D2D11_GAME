#pragma once

class Ball
{
public:
	Ball(class Context *context);
	virtual ~Ball();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }
	const Vector3 &GetJumpSpeed() const { return jumpSpeed; }
	const bool &GetCollResult() const { return bCollision; }
	const int &GetNumber() const { return ballNum; }
	const int &GetLevel() const { return level; }

	void SetCollider();

	void SetFloorSpeed(const float &floorSpeed) { this->floorSpeed = floorSpeed; }
	void SetJumpSpeed(const Vector3 &jumpSpeed) { this->jumpSpeed = jumpSpeed; }
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

	VertexShader *vertexShader;	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class Collider *collider;
	class AudioSource *pop_SE;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	ConstantBuffer *spriteBuffer;

	Vector3 position;
	Vector3 jumpSpeed;
	float jumpAccel;

	class Collider *opponent;
	CircleCheck bCheck;
	float floorSpeed;
	bool bCollision;
	bool bUpdate;
	int ballNum;
	int time;
	int level;
};