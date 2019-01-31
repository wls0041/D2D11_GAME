#include "stdafx.h"
#include "Rect.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Collider.h"

Rect::Rect(Context * context)
	: context(context)
{
	graphics = context->GetSubsystem<Graphics>();
	auto resourceMgr = context->GetSubsystem<ResourceManager>();

	GeometryUtility::CreateQuad(geometry);
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

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

	texture = resourceMgr->Load<Texture>("metalslug.png");

	transform = new Transform(context);

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &blendState);
		assert(SUCCEEDED(hr));
	}

	///////////////////////////////////////////////////////////////////

	animationBuffer = new ConstantBuffer(context);
	animationBuffer->Create<AnimationData>();

	animator = new Animator(context);

	animator->LoadFromFile("Player.xml");
	animator->SetCurrentAnimation("Idle");

	/////////////////////////////////////////////////////////////////////

	collider = new Collider(context);
	collider->SetTransform(transform);
	collider->Event = [this]()
	{
		assert(false);
	};

	//람다식, 람다함수, 무명함수
	//[변수캡쳐](파라미터)->리턴타입 { 함수 구현부 }(넘길인자)

	//변수캡쳐 : 현재 함수에서 사용하는 외부변수
	// = 을 넣으면 해당함수의 모든 변수를 전부 사용
	// & 을 넣으면 모든 변수를 참조로 받음
	// 비워두면 아무것도 사용하지 않음
	// 전역변수는 캠쳐할 필요가 없음

	// 파라미터 : 함수에서 받을 인자 ex) Add(int a, int b);
	// 리턴타입 : 리턴할 타입을 지정, 리턴값이 void 일때는 ->와 함께 생략가능
	// 함수 구현부 : 함수 몸체
	// 넘길인자 : 호출하는 함수에 넘겨줄 값 ex) Add(50, 70);
}

Rect::~Rect()
{
	SAFE_DELETE(transform);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Rect::Update()
{
	collider->SetCenter(transform->GetPosition());
	collider->SetSize({ 28, 38, 1 });

	auto data = static_cast<WorldData*>(worldBuffer->Map());
	data->World = transform->GetWorldMatrix();
	worldBuffer->Unmap();

	///////////////////////////////////////////
	auto input = context->GetSubsystem<Input>();
	if (input->KeyDown(VK_RIGHT))
		animator->SetCurrentAnimation("Run");
	else if (input->KeyDown(VK_LEFT))
		animator->SetCurrentAnimation("Idle");
	///////////////////////////////////////////

	animator->Update();

	auto animData = static_cast<AnimationData*>(animationBuffer->Map());
	animData->TextureSize = texture->GetSize();
	animData->SpriteOffset = animator->GetCurrentkeyframe()->offset;
	animData->SpriteSize = animator->GetCurrentkeyframe()->size;
	animationBuffer->Unmap();
}

void Rect::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	inputLayout->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	worldBuffer->BindPipeline(ShaderType::VS, 1);
	animationBuffer->BindPipeline(ShaderType::VS, 2);
	texture->BindPipeline(ShaderType::PS, 0);

	auto dc = graphics->GetDeviceContext();

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//dc->RSSetState(rsState);

	dc->OMSetBlendState(blendState, nullptr, 0xff);

	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0);
}
