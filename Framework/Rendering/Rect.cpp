#include "stdafx.h"
#include "Rect.h"
#include "../Scene/Component/Animator.h"

Rect::Rect(Context *context) : scale(1, 1, 1), position(0, 0, 0), rotate(0, 0, 0)
{
	graphics = context->GetSubsystem<Graphics>();
	auto resourceMgr = context->GetSubsystem<ResourceManager>();

	//Create Vertex Data, Create Index Data
	GeometryUtility::CreateQuad(geometry);

	//create vertex buffer
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	//create index buffer
	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("../_Assets/Shader/Animation.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/Animation.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	//Create Texture
	texture = resourceMgr->Load<Texture>("bird.png");

	//공간 단위행렬 초기화
	D3DXMatrixIdentity(&world);

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //앞면을 어떻게 판별할 것인가. false = 시계방향이 앞면
		desc.CullMode = D3D11_CULL_NONE; //컬링. 원하지 않는 부분(BACK, FRONT, NONE)은 생략해 퍼포먼스를 높임
		desc.FillMode = D3D11_FILL_WIREFRAME; //WIREFRAME. 외곽선만 출력

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = true; //alpha의 적용 범위
		desc.IndependentBlendEnable = false; //여러 rendertarget을 사용할 경우 각각의 state의 설정을 따로 쓰겠는가. false면 0의 설정을 모두 적용
		desc.RenderTarget[0].BlendEnable = false; //blend할 것인가


		//일단 사용. 색들의 혼합
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //혼합공식에 나오는 두 색을 어떻게 섞을 것인가(두 개를 더하라)

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//


		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //그리는 곳에 주어진 색만 써라 

		HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &blendState);
		assert(SUCCEEDED(hr));
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	animationBuffer = new ConstantBuffer(context);
	animationBuffer->Create<AnimationData>();

	animator = new Animator(context);

	//animator->RegisterAnimation("Idle.xml");
	//animator->SetCurrentAnimation("Idle");

	Animation *bird = new Animation(context);
	bird->SetResourceName("Bird");
	bird->AddKeyframe("bird.png", { 264, 90 }, { 17, 12 }, 200.0f);
	bird->AddKeyframe("bird.png", { 264, 64 }, { 17, 12 }, 200.0f);
	bird->AddKeyframe("bird.png", { 223, 124 }, { 17, 12 }, 200.0f);

	animator->RegisterAnimation(bird);
	animator->SetCurrentAnimation("Bird");

	animator->SaveToFile("Flappy_Bird.xml");
}

Rect::~Rect()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader); 
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Rect::Update()
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

	animator->Update();

	auto animData = static_cast<AnimationData*>(animationBuffer->Map());
	animData->TextureSize = texture->GetSize();
	animData->SpriteOffset = animator->GetCurrentkeyframe()->offset;
	animData->Spritesize = animator->GetCurrentkeyframe()->size;
	animationBuffer->Unmap();
}

void Rect::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	inputLayout->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	worldBuffer->BindPipeline(ShaderType::VS, 1); //0 : camera
	animationBuffer->BindPipeline(ShaderType::VS, 2);
	texture->BindPipeline(ShaderType::PS, 0);

	auto dc = graphics->GetDeviceContext();

	//IA단계 세팅
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅

	//RS단계 -> 기본 세팅 되어있음
	//dc->RSSetState(rsState);

	//PS단계

	//OM단계 -> back buffer를 가진 graphic클래스에서 OMSet을 해주고 있음
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //몇 개를, 몇 번부터
}
