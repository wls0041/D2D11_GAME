#pragma once

class Rect
{
public:
	Rect(class Context *context);
	virtual ~Rect();

	const D3DXVECTOR3 &GetScale() const { return scale; }
	const D3DXVECTOR3 &GetRotate() const { return rotate; }
	const D3DXVECTOR3 &GetPosition() const { return position; }

	void SetScale(const D3DXVECTOR3 &scale) { this->scale = scale; }
	void SetRotate(const D3DXVECTOR3 &rotate) { this->rotate = rotate; }
	void SetPosition(const D3DXVECTOR3 &position) { this->position = position; }

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

	D3DXVECTOR3 scale;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotate;
	D3DXMATRIX world; //�������

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	bool isRun;

	class Animator *animator;
	ConstantBuffer *animationBuffer;
};