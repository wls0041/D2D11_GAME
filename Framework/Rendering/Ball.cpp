#include "stdafx.h"
#include "Ball.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Collider.h"
#include "../Scene/Component/AudioSource.h"

Ball::Ball(Context *context) : context(context), position(0.0f), bCheck(CircleCheck::None), jumpSpeed(5.0f, -14.0f, 0.0f), floorSpeed(32.0f), jumpAccel(1.0f), bCollision(false), bUpdate(true), ballNum(0), time(0), level(0)
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

	pop_SE = new AudioSource(context);
}

Ball::~Ball()
{
	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->ClearCollider(to_string(ballNum));

	SAFE_DELETE(pop_SE);
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
	position = transform->GetPosition();
	collider->EventCircle = [this](const CircleCheck &check, Collider *opponent) {
		bCheck = check;
		this->opponent = opponent;

		Update();
	};
	collider->Event = [this]() { //bullet충돌
		bCollision = true;

		pop_SE->SetAudioClip("Pang_Pop.wav");
		pop_SE->Play();
		pop_SE->SetVolume(0.5f);
	};

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->RegisterCollider(to_string(ballNum), collider);
}

void Ball::InvMoveDir(const CircleCheck & check, Collider * opponent)
{
	Vector3 size = opponent->GetBoundBox().GetMax() - opponent->GetBoundBox().GetMin();
	Vector3 minLimit = 0.0f, maxLimit = 0.0f;
	bUpdate = false;

	if (size.x > 1000) { //벽과 충돌
		minLimit = opponent->GetBoundBox().GetMin() + transform->GetScale() * 0.5f;
		maxLimit = opponent->GetBoundBox().GetMax() - transform->GetScale() * 0.5f;

		if (check == CircleCheck::CollisionY) {
			if (position.y > maxLimit.y) position.y = maxLimit.y;
			else if (position.y < minLimit.y) {
				position.y = minLimit.y;
				jumpSpeed.y = floorSpeed * Math::Sign(jumpSpeed.y);
			}
			jumpSpeed.y = -jumpSpeed.y;
		}
		else if (check == CircleCheck::CollisionX) {
			if (position.x > maxLimit.x) position.x = maxLimit.x; 
			else if (position.x < minLimit.x) position.x = minLimit.x; 
			jumpSpeed.x = -jumpSpeed.x;
		}
		else if (check == CircleCheck::CollisionXY) {
			if (position.y > maxLimit.y) position.y = maxLimit.y;
			else if (position.y < minLimit.y) position.y = minLimit.y;
			if (position.x > maxLimit.x) position.x = maxLimit.x;
			else if (position.x < minLimit.x) position.x = minLimit.x;
			jumpSpeed.x = -jumpSpeed.x;
		}
	}
	else { //Block과 충돌
		CircleCheck subCheck = check; //Edge에 부딪혔을 때 튕길 방향 판정
		minLimit = opponent->GetBoundBox().GetMin() - transform->GetScale() * 0.5f;
		maxLimit = opponent->GetBoundBox().GetMax() + transform->GetScale() * 0.5f;

		if (subCheck == CircleCheck::CollisionEdge) { //모서리에 부딪히면 각도에 따라서 움직일 방향 결정
			Vector3 compCoord = position - opponent->GetCenter(); //원과 블록의 사이각을 구하기 위한 좌표

			float oppoDegree = Math::ToDegree(atan2(opponent->GetSize().y - opponent->GetCenter().y, opponent->GetSize().x - opponent->GetCenter().x)); //상대 중점과 상대 정점의 사이각
			float compDegree = Math::ToDegree(atan2(compCoord.y, compCoord.x)); //원과 블록 중점간의 사이각
			//minLimit = opponent->GetBoundBox().GetMin() - (100.0f * cosf(oppoDegree), 100.0f * sinf(oppoDegree));
			//maxLimit = opponent->GetBoundBox().GetMax() + (100.0f * cosf(oppoDegree), 100.0f * sinf(oppoDegree));

			if ((oppoDegree + 5 < compDegree && compDegree < 175 - oppoDegree) 
				|| -(175 - oppoDegree) < compDegree && compDegree < (-(oppoDegree + 5))) subCheck = CircleCheck::CollisionY;

			else if ((5 < compDegree && compDegree < oppoDegree - 5) || (175 - oppoDegree < compDegree && compDegree < 175) ||
				(-(oppoDegree - 5) < compDegree && compDegree < -5) || (-175 < compDegree && compDegree < -(175 - oppoDegree))) subCheck = CircleCheck::CollisionX;
			
			else subCheck = CircleCheck::CollisionXY;
		}
		if (subCheck == CircleCheck::CollisionY) {
			if (position.y > opponent->GetBoundBox().GetMax().y) position.y = maxLimit.y;
			else if (position.y < opponent->GetBoundBox().GetMin().y) position.y = minLimit.y;
			jumpSpeed.y = -jumpSpeed.y;
		}
		else if (subCheck == CircleCheck::CollisionX) {
			if (position.x > opponent->GetBoundBox().GetMax().x) position.x = maxLimit.x;
			else if (position.x < opponent->GetBoundBox().GetMin().x) position.x = minLimit.x;
			jumpSpeed.x = -jumpSpeed.x;
		}
		else if (subCheck == CircleCheck::CollisionXY) {
			/*if (position.y > opponent->GetBoundBox().GetMax().y) position.y = maxLimit.y;
			else if (position.y < opponent->GetBoundBox().GetMin().y) position.y = minLimit.y;
			if (position.x > opponent->GetBoundBox().GetMax().x) position.x = maxLimit.x;
			else if (position.x < opponent->GetBoundBox().GetMin().x) position.x = minLimit.x;*/
			jumpSpeed = jumpSpeed * -1.0f;
		}
	}
}

void Ball::Update()
{
	////////////////////공의 이동////////////////////////
	if (bCheck != CircleCheck::None) {
		InvMoveDir(bCheck, opponent);
	}
	if (bUpdate) {
		jumpSpeed.y += jumpAccel;
		position.y -= jumpSpeed.y;
		position.x += jumpSpeed.x;
	}

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

	bCheck = CircleCheck::None;
	bUpdate = true;
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
