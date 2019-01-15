#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "../Rendering/Rect.h"
#include "../Resource/AudioClip.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	rect = new Rect(context);
	rect->SetScale({ 1, 1, 1 });

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("Stage1.mp3");
	clip->Play();
}

Scene::~Scene()
{
	for (auto audioClip : clips) SAFE_DELETE(audioClip);

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
	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("Stage1.mp3");
	
	if (input->KeyDown('Q')) clip->Play();
	else if (input->KeyDown('W')) clip->Pause();
	else if (input->KeyDown('E')) clip->Stop();


	/////////////////////오디오 복사 생성자 테스트코드//////////////////////////
	static auto shoot = resourceMgr->Load<AudioClip>("Shoot1.wav");
	
	if (input->BtnDown(0)) { //좌클릭
		clips.push_back(new AudioClip(*shoot));
		clips.back()->Play();
	}

}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	rect->Render();
}
