#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Player.h"
#include "../Rendering/Ball.h"
#include "../Rendering/Back.h"
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
	ball->GetTransform()->SetScale({ 200, 200, 1 });
	ball->GetTransform()->SetPosition({ 0, 50, 0 });
	ball->SetMoveDir({ 1.0f, -1.0f, 0.0f });
	ball->SetCollider();

	back = new Back(context);
	back->GetTransform()->SetScale({ 1280, 720, 1 });
	back->GetTransform()->SetPosition({ 0, 0, 0 });
	back->SetOffset({ 8, 1520 });
	back->SetCollider();

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	//bgm->Play();

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->RegisterCollider("Player", player->GetCollider());
	colliderMgr->RegisterCollider("Ball", ball->GetCollider());
	colliderMgr->RegisterCollider("Back", back->GetCollider());
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);

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

	auto colliderMgr = context->GetSubsystem <ColliderManager>();
	ball->SetCurCheck(true);
	colliderMgr->HitCheck_AABB("Back", "Ball", 2); //Ball이 배경 밖으로 나가는가(x측)

	ball->SetCurCheck(false);
	colliderMgr->HitCheck_AABB("Back", "Ball", 3); //Ball이 배경 밖으로 나가는가(y축)

	colliderMgr->HitCheck_AABB("Ball", "Player", 1); //Ball Player충돌

	ball->Update();
	player->Update();
	back->Update();

}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	ball->Render();
	player->Render();

	auto dw = context->GetSubsystem<DirectWrite>();
	char tmp[1000] = { 0 };
	itoa(player->GetLife(), tmp, 10); 
	dw->Text(reinterpret_cast<wchar_t*>(tmp), Vector2(0, 700), 100.0f, Color(1, 1, 0, 1));
	
	back->Render();
}
