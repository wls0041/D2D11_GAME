#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Player.h"
#include "../Rendering/Ball.h"
#include "../Rendering/Back.h"
#include "../Rendering/Block.h"
#include "../Scene/Component/Transform.h"
#include "./Component/Collider.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	player = new Player(context);
	player->GetTransform()->SetScale({ 3.0f, 3.0f, 1 });
	player->GetTransform()->SetPosition({ 0, -360 + 76.0f, 0 });
	player->SetCollider();

	ball = new Ball(context);
	ball->GetTransform()->SetScale({ 160, 160, 1 });
	ball->GetTransform()->SetPosition({ -300, 150, 0 });
	ball->SetMoveDir({ 1.0f, -1.0f, 0.0f });
	ball->SetCollider();

	back = new Back(context);
	back->GetTransform()->SetScale({ 1280, 720, 1 });
	back->GetTransform()->SetPosition({ 0, 0, 0 });
	back->SetOffset({ 8, 1520 });
	back->SetCollider();
	back->Update();

	block = new Block(context);
	block->GetTransform()->SetScale({ 100, 50, 1 });
	block->GetTransform()->SetPosition({ 0, 0, 0 });
	block->SetOffset({ 8, 1520 });
	block->SetCollider();
	block->Update();

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	//bgm->Play();

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->RegisterCollider("Player", player->GetCollider());
	colliderMgr->RegisterCollider("Ball", ball->GetCollider());
	colliderMgr->RegisterCollider("Back", back->GetCollider());
	colliderMgr->RegisterCollider("Block", block->GetCollider());
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);

	SAFE_DELETE(back);
	SAFE_DELETE(ball);
	SAFE_DELETE(player);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(camera);
}

void Scene::Update()
{
	camera->Update();
	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	cameraData->View = camera->GetViewMatrix();
	cameraData->Projection = camera->GetProjectionMatrix();
	cameraBuffer->Unmap();

	auto input = context->GetSubsystem<Input>();

	ball->Update();
	player->Update();

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->HitCheck_AABB_Circle("Back", "Ball", CheckCase::Back_Circle); //Ball이 배경 밖으로 나가는가
	//colliderMgr->HitCheck_AABB_Circle("Block", "Ball", CheckCase::Circle_Rect); //Ball Block충돌
	colliderMgr->HitCheck_AABB_Circle("Ball", "Player", CheckCase::Circle_Rect); //Ball Player충돌

}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	ball->Render();
	player->Render();
	block->Render();

	auto dw = context->GetSubsystem<DirectWrite>();
	dw->Text(to_wstring(player->GetLife()), Vector2(0, 700), 100.0f, Color(1, 1, 0, 1));
	
	back->Render();
}
