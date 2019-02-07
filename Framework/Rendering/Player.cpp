#include "stdafx.h"
#include "Player.h"
#include "../Scene/Component/Animator.h"
#include "../Scene/Component/Transform.h"
#include "../Scene/Component/Collider.h"
#include "../Scene/Component/AudioSource.h"
#include "Bullet.h"

Player::Player(Context *context) : context(context), jumpSpeed(2.0f, 8.0f, 0.0f), jumpAccel(0.3f), life(5), x_clamp(0), bBullet(false), bLive(true), bDeadFinish(false), timeLimit(60.1f), deadTimer(0), color(1.0f)
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
	vertexShader->Create("../_Assets/Shader/Pang/Animation_Player.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/Pang/Animation_Player.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	//Create Texture
	texture = resourceMgr->Load<Texture>("Pang_Player.png");

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
	animationBuffer = new ConstantBuffer(context);
	animationBuffer->Create<AnimationData>();

	animator = new Animator(context);

	//animator->RegisterAnimation("Idle.xml");
	animator->LoadFromFile("Pang_Player.xml");
	animator->SetCurrentAnimation("Idle");

	//Set transform(scale, position, rotation, world)
	transform = new Transform(context);

	x_clamp = (1280.0f - 150.0f) * 0.5f;

	shoot_SE = new AudioSource(context);
	death_SE = new AudioSource(context);
	death_SE->SetAudioClip("Pang_Death.mp3");
}

Player::~Player()
{
	SAFE_DELETE(death_SE);
	SAFE_DELETE(shoot_SE);
	SAFE_DELETE(collider);
	SAFE_DELETE(transform);
	SAFE_DELETE(animator);
	SAFE_DELETE(animationBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}


const Vector3 & Player::GetSize()
{
	Vector3 size = { animator->GetCurrentkeyframe()->size.x, animator->GetCurrentkeyframe()->size.y, 0 };
	return Vector3(transform->GetScale() * size);
}

void Player::SetCollider()
{	
	//Collider
	collider = new Collider(context);
	collider->SetCenter(transform->GetPosition());
	collider->SetSize(GetSize());
	collider->SetTransform(transform);
	collider->EventPlayer = [this](const float &pos) { //���ٽ�.�����Լ�. ������ �Լ�, �������� [this]()->void
		LoseLife(pos);
	};
}

void Player::LoseLife(const float &pos)
{
	float position = transform->GetPosition().x;
	int dir;

	position > pos ? dir = 1 : dir = -1;

	Vector3 scale = transform->GetScale();
	scale.x = dir * scale.x * Math::Sign(scale.x);
	transform->SetScale(scale);

	jumpSpeed.x *= Math::Sign(scale.x);

	bLive = false;
	death_SE->Play();
	death_SE->SetVolume(0.5f);
	deadTimer = 0;

	life--;
}

void Player::ShootBullet()
{
	Vector3 position = transform->GetPosition();

	bullet = new Bullet(context);
	bullet->GetTransform()->SetScale({ 2.0f, 3.7f, 1 });
	bullet->GetTransform()->SetPosition({ position.x, position.y - GetSize().y * 0.5f, 0 });
	bullet->SetCollider();

	bullet->SetExist(true);
	bBullet = true;
}

void Player::Update()
{
	///////////////////////////////////////////
	auto input = context->GetSubsystem<Input>();

	Vector3 position = transform->GetPosition();
	Vector3 scale = transform->GetScale();
	bool bShoot = false;

	if (bLive) {
		if (input->KeyDown('T')) {
			timeLimit = 3;
			color = { 1.0f, 1.0f, 0.0f, 1.0f };
		}

		if (input->KeyDown(VK_SPACE) && !bBullet) { //����� ��ݸ��
			animator->SetCurrentAnimation("Shoot");
			bShoot = true;
			deadTimer = 0;
		}
		if (input->KeyPress(VK_RIGHT)) {
			animator->SetCurrentAnimation("Move");
			position.x += 8.0f;
			if (scale.x < 0) scale.x = -scale.x;
		}
		else if (input->KeyPress(VK_LEFT)) {
			animator->SetCurrentAnimation("Move");
			position.x -= 8.0f;
			if (scale.x > 0) scale.x = -scale.x;
		}
		else animator->SetCurrentAnimation("Idle");

		position.x = Math::clamp(position.x, -x_clamp, x_clamp);

		if (bShoot) {
			ShootBullet();
			shoot_SE->SetAudioClip("Pang_Shoot.mp3");
			shoot_SE->Play();
			shoot_SE->SetVolume(0.45f);
		}
		if (bBullet) {
			bullet->Update();

			auto colliderMgr = context->GetSubsystem<ColliderManager>();
			colliderMgr->HitCheck_AABB("Block", "Bullet"); //Ball Block�浹

			if (!bullet->GetExist()) {
				SAFE_DELETE(bullet);
				bBullet = false;
			}
		}

		timeLimit -= context->GetSubsystem<Timer>()->GetDeltaTimeSec();
		if (timeLimit <= 0) {
			LoseLife(0.0f);
			scale = transform->GetScale();
		}
	}
	else {
		deadTimer++;
		animator->SetCurrentAnimation("Die");
		if (deadTimer > 30 && deadTimer < 120) {
			jumpSpeed.y -= jumpAccel;
			position += jumpSpeed;
		}
		else if (deadTimer > 120) {
			bDeadFinish = true;
		}
	}
	transform->SetPosition(position);
	transform->SetScale(scale);

	collider->SetCenter(position);

	///////////////////////////////////////////

	///////////////////////////////////////////
	auto data = static_cast<WorldData*>(worldBuffer->Map());
	data->World = transform->GetWorldMatrix();
	worldBuffer->Unmap();

	animator->Update();

	auto animData = static_cast<AnimationData*>(animationBuffer->Map());
	animData->TextureSize = texture->GetSize();
	animData->SpriteOffset = animator->GetCurrentkeyframe()->offset;
	animData->SpriteSize = animator->GetCurrentkeyframe()->size;
	animationBuffer->Unmap();
}

void Player::Render()
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

	if (bBullet) bullet->Render();

	auto dw = context->GetSubsystem<DirectWrite>();
	dw->Text(L"Time : " + to_wstring(static_cast<int>(timeLimit)), Vector2(1000, 780), 50.0f, color);
}