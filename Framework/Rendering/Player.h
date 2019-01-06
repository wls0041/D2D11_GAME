#pragma once

class Player
{
public:
	Player(class Context *context);
	virtual ~Player();

	const D3DXVECTOR3 &GetScale() const { return scale; }
	const D3DXVECTOR3 &GetRotate() const { return rotate; }
	const D3DXVECTOR3 &GetPosition() const { return position; }
	const bool &GetJump() const { return IsJump; }
	const float &GetJumpTime() const { return JumpTime; }

	void SetScale(const D3DXVECTOR3 &scale) { this->scale = scale; }
	void SetRotate(const D3DXVECTOR3 &rotate) { this->rotate = rotate; }
	void SetPosition(const D3DXVECTOR3 &position) { this->position = position; }
	void SetJump(const bool &IsJump) { this->IsJump = IsJump; }
	void SetJumpTime(const float &JumpTime) { this->JumpTime = JumpTime; }

	void Update();
	void Render();

private:
	class Graphics *graphics;

	Geometry<VertexColor> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	VertexShader *vertexShader;	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;
	Texture *texture;

	D3DXVECTOR3 scale;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotate;
	D3DXMATRIX world; //�������

	bool IsJump;
	float JumpTime;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)
};