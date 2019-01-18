#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Rect.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	rect = new Rect(context);
	rect->SetScale({ 1, 1, 1 });

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	bgm->Play();
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);

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

	auto input = context->GetSubsystem<Input>();

	if (input->KeyDown('Q')) bgm->Play();
	else if (input->KeyDown('W')) bgm->Pause();
	else if (input->KeyDown('E')) bgm->Stop();


	///////////////////////////////////////////////
	static float volume = 1.0f;
	if (input->KeyDown('A')) volume -= 0.1f;
	else if (input->KeyDown('S')) volume += 0.1f;

	bgm->SetVolume(volume);

	///////////////////////////////////////////////
	static float pitch = 1.0f;
	if (input->KeyDown('Z')) pitch += 0.1f;
	else if (input->KeyDown('X')) pitch -= 0.1f;

	bgm->SetPitch(pitch);

}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	rect->Render();
}
