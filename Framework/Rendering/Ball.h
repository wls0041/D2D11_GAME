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

	VertexShader *vertexShader;	//hlsl�� VS, PS�� ���������ο� �� �� �ֵ��� ��ȯ
	PixelShader *pixelShader;

	InputLayout *inputLayout; //�迭 vertexColor*�� ������ �ֱ� ����
	ConstantBuffer *worldBuffer;
	Texture *texture;

	class Transform *transform;
	class BoundBox *boundbox;

	ID3D11RasterizerState *rsState;
	ID3D11BlendState *blendState; //OM�ܰ迡 ��(�̹� ���� ������ �־�� �ϱ� ����)

	ConstantBuffer *spriteBuffer;

	Vector3 moveDir; //���� x, y�������� �����, ���������� ����
	Vector3 minBox;
	Vector3 maxBox;
};