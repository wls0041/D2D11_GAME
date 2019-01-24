#pragma once

class Rect
{
public:
	Rect(class Context *context);
	virtual ~Rect();

	const Vector3 &GetScale() const { return scale; }
	const Vector3 &GetRotate() const { return rotate; }
	const Vector3 &GetPosition() const { return position; }

	void SetScale(const Vector3 &scale) { this->scale = scale; }
	void SetRotate(const Vector3 &rotate) { this->rotate = rotate; }
	void SetPosition(const Vector3 &position) { this->position = position; }

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

	Vector3 scale;
	Vector3 position;
	Vector3 rotate;
	Matrix world; //�������

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	bool isRun;

	class Animator *animator;
	ConstantBuffer *animationBuffer;
};