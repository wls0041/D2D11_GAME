#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Player.h"
#include "../Scene/Component/Transform.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	player = new Player(context);
	player->GetTransform()->SetScale({ 1.5f, 1.5f, 1 });

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	bgm->Play();
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);

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

	player->Update();

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
	////////////////////////////////////////////////////////////////
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	player->Render();
}
