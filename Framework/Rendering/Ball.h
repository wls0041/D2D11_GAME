#pragma once

class Ball
{
public:
	Ball(class Context *context);
	virtual ~Ball();

	class Transform *GetTransform() const { return transform; }
	class BoundBox *GetBoundBox() const { return boundbox; }

	void SetMoveDir(const Vector3 &moveDir) { this->moveDir = moveDir; }
	void InvMoveDir() { moveDir.x = -moveDir.x; moveDir.y = -moveDir.y; moveDir.z = -moveDir.z; }

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
	class BoundBox *boundbox;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM단계에 들어감(이미 색이 찍혀져 있어야 하기 때문)

	ConstantBuffer *spriteBuffer;

	Vector3 moveDir; //공의 x, y움직임이 양방향, 음방향인지 결정
	Vector3 minBox;
	Vector3 maxBox;
};