#pragma once

class Anim
{
public:
	Anim(class Context *context);
	virtual ~Anim();

	const D3DXVECTOR3 &GetScale() const { return scale; }
	const D3DXVECTOR3 &GetRotate() const { return rotate; }
	const D3DXVECTOR3 &GetPosition() const { return position; }

	void SetScale(const D3DXVECTOR3 &scale) { this->scale = scale; }
	void SetRotate(const D3DXVECTOR3 &rotate) { this->rotate = rotate; }
	void SetPosition(const D3DXVECTOR3 &position) { this->position = position; }

	void SetOffset(const D3DXVECTOR2 &offset) { this->offset = offset; }
	void SetSize(const D3DXVECTOR2 &size) { this->size = size; }

	void Update();
	void Render();

private:
	class Graphics *graphics;

	Geometry<VertexTexture> geometry;
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;

	VertexShader *vertexShader;	//hlsl의 VS, PS가 파이프라인에 들어갈 수 있도록 변환
	PixelShader *pixelShader;

	InputLayout *inputLayout; //배열 vertexColor*에 정보를 주기 위함
	ConstantBuffer *worldBuffer;
	Texture *texture;

	D3DXVECTOR3 scale;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotate;

	D3DXVECTOR2 size;
	D3DXVECTOR2 offset;

	D3DXMATRIX world; //세계공간

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	ConstantBuffer *spriteBuffer;
};