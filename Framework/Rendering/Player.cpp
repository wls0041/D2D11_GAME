#include "stdafx.h"
#include "Player.h"

Player::Player(Context *context) : scale(1, 1, 1), position(-300, 100, 0), rotate(0, 0, 0), IsJump(false), JumpTime(0.0f)
{
	graphics = context->GetSubsystem<Graphics>();
	auto resourceMgr = context->GetSubsystem<ResourceManager>();

	//Create Vertex Data, Create Index Data
	GeometryUtility::CreateCircle(geometry);

	//create vertex buffer
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexColor>(geometry.GetVertices());

	//create index buffer
	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("../_Assets/Shader/Color.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/Color.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	//���� ������� �ʱ�ȭ
	D3DXMatrixIdentity(&world);

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //�ո��� ��� �Ǻ��� ���ΰ�. false = �ð������ �ո�
		desc.CullMode = D3D11_CULL_NONE; //�ø�. ������ �ʴ� �κ�(BACK, FRONT, NONE)�� ������ �����ս��� ����
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. �ܰ����� ���

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}
}

Player::~Player()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Player::Update()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotate.y, rotate.x, rotate.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * R * T;

	auto data = static_cast<WorldData*>(worldBuffer->Map());
	//��켱�� ���켱���� �ٲ���(��ġ)
	D3DXMatrixTranspose(&data->World, &world);
	worldBuffer->Unmap();
}

void Player::Render()
{
	auto dc = graphics->GetDeviceContext();

	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	inputLayout->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	worldBuffer->BindPipeline(ShaderType::VS, 1); //0 : camera

	//IA�ܰ� ����
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	dc->RSSetState(rsState);

	//PS�ܰ�

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //�� ����, �� ������
}
