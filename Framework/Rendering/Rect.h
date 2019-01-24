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

	VertexShader *vertexShader;	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	PixelShader *pixelShader;

	InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	ConstantBuffer *worldBuffer;
	Texture *texture;

	Vector3 scale;
	Vector3 position;
	Vector3 rotate;
	Matrix world; //세계공간

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	bool isRun;

	class Animator *animator;
	ConstantBuffer *animationBuffer;
};