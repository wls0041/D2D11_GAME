#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Player.h"
#include "../Rendering/Ball.h"
#include "../Rendering/Back.h"
#include "../Scene/Component/Transform.h"
#include "../Math/BoundBox.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	player = new Player(context);
	player->GetTransform()->SetScale({ 1.5f, 1.5f, 1 });
	player->GetTransform()->SetPosition({ 0, -360 + 50.0f, 0 });

	ball = new Ball(context);
	ball->GetTransform()->SetScale({ 200, 200, 1 });
	ball->GetTransform()->SetPosition({ 0, 50, 0 });
	ball->SetMoveDir({ 1.0f, 0.0f, 0.0f });

	back = new Back(context);
	back->GetTransform()->SetScale({ 1280, 720, 1 });
	back->GetTransform()->SetPosition({ 0, 0, 0 });
	back->SetOffset({ 8, 1520 });

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	//bgm->Play();
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

	Intersection inter = ball->GetBoundBox()->IsCircleInside(*back->GetBoundBox());
	if (inter == Intersection::Outside) ball->InvMoveDir();

	ball->Update();
	player->Update();
	back->Update();

}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	ball->Render();
	player->Render();
	back->Render();
}
