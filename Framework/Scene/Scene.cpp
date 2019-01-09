#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "../Rendering/Rect.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	rect = new Rect(context);
	rect->SetScale({ 100, 100, 1 });
}

Scene::~Scene()
{
	SAFE_DELETE(rect);
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

	rect->Update();
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	rect->Render();
}
