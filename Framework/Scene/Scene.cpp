#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Rect.h"
#include "../Scene/Component/Transform.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	rect = new Rect(context);
	//rect->SetScale({ 1, 1, 1 });
	rect1 = new Rect(context);
	rect1->GetTransform()->SetPosition({ 50, 50, 0 });
	rect1->GetTransform()->SetParent(rect->GetTransform());

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	bgm = new AudioSource(context);
	bgm->SetAudioClip("Stage1.mp3");
	bgm->Play();
}

Scene::~Scene()
{
	for (auto source : sources) SAFE_DELETE(source);

	SAFE_DELETE(rect1);
	SAFE_DELETE(rect);
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

	static Vector3 rotate;
	if(input->KeyPress(VK_SPACE)) rotate.z += 0.2f;
	rect->GetTransform()->SetRotation(rotate);

	Vector3 position = rect->GetTransform()->GetPosition();
	if (input->KeyPress(VK_RIGHT)) {
		position.x += 0.1f;
	}
	rect->GetTransform()->SetPosition(position);

	rect->Update();
	rect1->Update();

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
	rect->Render();
	rect1->Render();

	auto dw = context->GetSubsystem<DirectWrite>();
	dw->Text(L"Hello, DirectWrite!!", Vector2(0, 0));
	dw->Text(L"수업끝!!", Vector2(100, 50), 50.0f);
	dw->Text(L"컴퓨터 이상함!!", Vector2(200, 100), 50.0f, Color(1, 1, 0, 1), L"궁서체");

}
