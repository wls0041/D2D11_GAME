#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "../Rendering/Rect.h"
#include "../Rendering/Anim.h"
#include "../Resource/AudioClip.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	anim = new Anim(context);
	anim->SetScale({ 1, 1, 1 });
	anim->SetOffset({ 0, 0 });
	anim->SetSize({ 143, 256 });

	bird = new Rect(context);
	bird->SetScale({ 2.5f, 2.5f, 1 });
	bird->SetPosition({ -200, 0, 0 });

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("Stage1.mp3");
	clip->Play();
}

Scene::~Scene()
{
	SAFE_DELETE(bird);
	SAFE_DELETE(anim);
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(camera);
}

void Scene::Update()
{
	camera->Update();
	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	D3DXMatrixTranspose(&cameraData->View, &camera->GetViewMatrix()); //쉐이더는 열우선이므로 전치 필요
	D3DXMatrixTranspose(&cameraData->Projection, &camera->GetProjectionMatrix());
	cameraBuffer->Unmap();

	anim->Update();
	bird->Update();
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	anim->Render();
	bird->Render();
}
