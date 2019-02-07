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

	VertexShader *vertexShader;	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class Collider *collider;
	class Bullet *bullet;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

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