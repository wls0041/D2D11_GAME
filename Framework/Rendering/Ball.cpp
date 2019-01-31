#include "stdafx.h"
#include "Ball.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Collider.h"

Ball::Ball(Context *context) : context(context), moveDir(0.0f), jumpSpeed(12.0f), jumpAccel(0.5f), tempPos(0.0f), curCheck_X(true), bRepos(false), canMoveY(true)
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
	vertexShader->Create("../_Assets/Shader/Pang/Sprite_Ball.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/Pang/Sprite_Ball.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	//Create Texture
	texture = resourceMgr->Load<Texture>("Pang_Bollon.png");

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
	spriteBuffer = new ConstantBuffer(context);
	spriteBuffer->Create<AnimationData>();

	//Set transform(scale, position, rotation, world)
	transform = new Transform(context);
}

Ball::~Ball()
{
	SAFE_DELETE(collider);
	SAFE_DELETE(transform);
	SAFE_DELETE(spriteBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Ball::SetCollider()
{
	//Collider
	collider = new Collider(context);
	collider->SetCenter(transform->GetPosition());
	collider->SetSize(transform->GetScale());
	collider->SetTransform(transform);
	collider->Event = [this]() { //람다식.람다함수. 무명의 함수, 정식형태 [this]()->void
		InvMoveDir();	
	};
}

void Ball::InvMoveDir()
{
	Vector3 position = transform->GetPosition();

	//충돌 후 한틱의 움직임이 있어 Collision 버그가 발생. 한틱의 움직임을 되돌림
	curCheck_X == true ? moveDir.x = -moveDir.x : moveDir.y = -moveDir.y; 
	curCheck_X == true ? position.x += 4.0f * moveDir.x : position.y += jumpSpeed * moveDir.y; //canMoveY = false; 
	//if (curCheck_X == false && moveDir.y < 0) 
	//	bRepos = true;

	transform->SetPosition(position);
	collider->SetCenter(position);

}

void Ball::Update()
{
	//if (moveDir.y < 0 && jumpSpeed <= 0.0f) {
	//	curCheck_X = false;
	//	jumpSpeed = -jumpSpeed;
	//	InvMoveDir();
	//}
	//float speed = transform->GetPosition.y


	////////////////////공의 이동////////////////////////
	//jumpSpeed < 0 ? moveDir.y = -1.0f : moveDir.y = 1.0f;
	jumpSpeed -= jumpAccel * moveDir.y;

	Vector3 position = transform->GetPosition();
	position.x += 4.0f * moveDir.x;
	position.y += jumpSpeed * moveDir.y;

	collider->SetCenter(position);

	transform->SetPosition(position);

	auto data = static_cast<WorldData*>(worldBuffer->Map());
	data->World = transform->GetWorldMatrix();
	worldBuffer->Unmap();

	auto animData = static_cast<AnimationData*>(spriteBuffer->Map());
	animData->TextureSize = texture->GetSize();
	animData->SpriteOffset = Vector2(0, 5);
	animData->SpriteSize = Vector2(50, 42);
	spriteBuffer->Unmap();
}

void Ball::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	inputLayout->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	worldBuffer->BindPipeline(ShaderType::VS, 1); //0 : camera
	spriteBuffer->BindPipeline(ShaderType::VS, 2);
	texture->BindPipeline(ShaderType::PS, 0);

	auto dc = graphics->GetDeviceContext();

	//IA단계 세팅
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅

	//RS단계 -> 기본 세팅 되어있음
	dc->RSSetState(rsState);

	//PS단계

	//OM단계 -> back buffer를 가진 graphic클래스에서 OMSet을 해주고 있음
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //몇 개를, 몇 번부터
}
