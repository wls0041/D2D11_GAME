#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Rect.h"
#include "../Rendering/Player.h"
#include "../Rendering/Back.h"
#include "../Rendering/Block.h"
#include "../Rendering/BallManager.h"
#include "../Scene/Component/Transform.h"
#include "./Component/Collider.h"

Scene::Scene(class Context *context, const int &life) : context(context), bDeadFinish(false), bClear(false), bStart(false), bLive(true), life(life), time(0)
{
	auto resourceMgr = context->GetSubsystem<ResourceManager>();

	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	message = new Rect(context, "Message_Ready.png");
	message->GetTransform()->SetScale({ 100.0f * 4.5f, 100.0f, 1 });
	message->GetTransform()->SetPosition({ 0, 25, 0 });
	message->Update();

	player = new Player(context);
	player->GetTransform()->SetScale({ 3.0f, 3.0f, 1 });
	player->GetTransform()->SetPosition({ 0, -360 + 76.0f, 0 });
	player->SetCollider();
	player->SetLife(life);
	player->Update();

	back = new Back(context);
	back->GetTransform()->SetScale({ 1280, 720, 1 });
	back->GetTransform()->SetPosition({ 0, 0, 0 });
	back->SetOffset({ 8, 1520 });
	back->SetCollider();
	back->Update();

	block = new Block(context);
	block->GetTransform()->SetScale({ 100, 50, 1 });
	block->GetTransform()->SetPosition({ 0, 0, 0 });
	block->SetCollider();
	block->Update();
	
	ballManager = new BallManager(context);
	ballManager->SetLevel(4);
	ballManager->SetPosition({ -300, 150, 0 });
	ballManager->SetMoveDir({ 1.0f, -1.0f, 0.0f });
	ballManager->SetFirstBall();
	ballManager->Update();

	bgm = new AudioSource(context);
	bgm->SetAudioClip("Pang_BGM.wav");

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->RegisterCollider("Player", player->GetCollider());
	colliderMgr->RegisterCollider("Back", back->GetCollider());
	colliderMgr->RegisterCollider("Block", block->GetCollider());
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);
	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->ClearCollider("Player");
	colliderMgr->ClearCollider("Back");
	colliderMgr->ClearCollider("Block");

	SAFE_DELETE(ballManager);
	SAFE_DELETE(bgm);
	SAFE_DELETE(block);
	SAFE_DELETE(back);
	SAFE_DELETE(player);
	SAFE_DELETE(message);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(camera);
}

void Scene::Update()
{
	bgm->Play();
	bgm->SetVolume(0.4f);

	camera->Update();
	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	cameraData->View = camera->GetViewMatrix();
	cameraData->Projection = camera->GetProjectionMatrix();
	cameraBuffer->Unmap();

	auto input = context->GetSubsystem<Input>();
	life = player->GetLife();

	if (time > 120) bStart = true;
	else time++;

	if (bStart) {
		bLive = player->IsLive();
		bDeadFinish = player->IsFinish();
		if(bDeadFinish) bgm->Stop();

		if (bLive) {
			ballManager->Update();
			bClear = ballManager->IsClear();
		}
		player->Update();
	}
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);

	if (!bStart) {
		float judge = time % 40;
		if(judge > 15 && judge < 40) message->Render();
	}
	player->Render();
	ballManager->Render();
	block->Render();
	back->Render();

	auto dw = context->GetSubsystem<DirectWrite>();
	dw->Text(L"Life : " + to_wstring(life), Vector2(40, 780), 50.0f, Color(1, 1, 1, 1));
}
