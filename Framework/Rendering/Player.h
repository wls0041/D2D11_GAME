#pragma once

class Player
{
public:
	Player(class Context *context);
	virtual ~Player();

	class Transform *GetTransform() const { return transform; }
	class Collider *GetCollider() const { return collider; }
	const bool &IsLive() const { return bLive; }
	const bool &IsFinish() const { return bDeadFinish; }
	const int &GetLife() const { return life; }
	const Vector3 &GetSize();

	void LoseLife(const float &pos);

	void SetLife(const int &life) { this->life = life; }
	void SetCollider();
	void ShootBullet();

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
	class Bullet *bullet;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	class Animator *animator;
	class AudioSource *shoot_SE;
	class AudioSource *death_SE;

	ConstantBuffer *animationBuffer;

	Vector3 jumpSpeed;
	Color color;
	float jumpAccel;
	float x_clamp;
	float timeLimit;
	bool bBullet;
	bool bLive;
	bool bDeadFinish;
	int deadTimer;
	int life;
};