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

	//공간 단위행렬 초기화
	D3DXMatrixIdentity(&world);

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //앞면을 어떻게 판별할 것인가. false = 시계방향이 앞면
		desc.CullMode = D3D11_CULL_NONE; //컬링. 원하지 않는 부분(BACK, FRONT, NONE)은 생략해 퍼포먼스를 높임
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. 외곽선만 출력

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
	//행우선을 열우선으로 바꿔줌(전치)
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

	//IA단계 세팅
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅

	//RS단계 -> 기본 세팅 되어있음
	dc->RSSetState(rsState);

	//PS단계

	//OM단계 -> back buffer를 가진 graphic클래스에서 OMSet을 해주고 있음

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //몇 개를, 몇 번부터
}
