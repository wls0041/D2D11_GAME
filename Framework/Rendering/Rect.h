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

	void SaveCurPosition(const float &x, const float &y) { curposition.x = x; curposition.y = y; }
	void SaveCurPosition_x(const float &x) { curposition.x = x; }
	void SaveCurPosition_y(const float &y) { curposition.y = y; }

	void Update();
	void Render();

	void Pause();
	void CalcTheta();

private:
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
	D3DXVECTOR3 curposition;
	D3DXVECTOR3 rotate;
	D3DXMATRIX world; //�������

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	class Animator *animator;
	ConstantBuffer *animationBuffer;
};