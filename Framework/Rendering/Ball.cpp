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
		desc.FrontCounterClockwise = false; //�ո��� ��� �Ǻ��� ���ΰ�. false = �ð������ �ո�
		desc.CullMode = D3D11_CULL_NONE; //�ø�. ������ �ʴ� �κ�(BACK, FRONT, NONE)�� ������ �����ս��� ����
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. �ܰ����� ���

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = true; //alpha�� ���� ����
		desc.IndependentBlendEnable = false; //���� rendertarget�� ����� ��� ������ state�� ������ ���� ���ڴ°�. false�� 0�� ������ ��� ����
		desc.RenderTarget[0].BlendEnable = false; //blend�� ���ΰ�


		//�ϴ� ���. ������ ȥ��
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //ȥ�հ��Ŀ� ������ �� ���� ��� ���� ���ΰ�(�� ���� ���϶�)

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //�׸��� ���� �־��� ���� ��� 

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
	collider->Event = [this]() { //bullet�浹
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

	if (size.x > 1000) { //���� �浹
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
	else { //Block�� �浹
		CircleCheck subCheck = check; //Edge�� �ε����� �� ƨ�� ���� ����
		minLimit = opponent->GetBoundBox().GetMin() - transform->GetScale() * 0.5f;
		maxLimit = opponent->GetBoundBox().GetMax() + transform->GetScale() * 0.5f;

		if (subCheck == CircleCheck::CollisionEdge) { //�𼭸��� �ε����� ������ ���� ������ ���� ����
			Vector3 compCoord = position - opponent->GetCenter(); //���� ����� ���̰��� ���ϱ� ���� ��ǥ

			float oppoDegree = Math::ToDegree(atan2(opponent->GetSize().y - opponent->GetCenter().y, opponent->GetSize().x - opponent->GetCenter().x)); //��� ������ ��� ������ ���̰�
			float compDegree = Math::ToDegree(atan2(compCoord.y, compCoord.x)); //���� ��� �������� ���̰�
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
	////////////////////���� �̵�////////////////////////
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

	//IA�ܰ� ����
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	dc->RSSetState(rsState);

	//PS�ܰ�

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //�� ����, �� ������
}
