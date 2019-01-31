#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "./Component/AudioSource.h"
#include "../Rendering/Rect.h"
#include "../Scene/Component/Transform.h"
#include "./Component/Collider.h"

Scene::Scene(class Context *context) : context(context)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	//rect->SetScale({ 1, 1, 1 });
	rect = new Rect(context);
	rect1 = new Rect(context);
	rect1->GetTransform()->SetPosition({ 50, 0, 0 });

	auto colliderMgr = context->GetSubsystem<ColliderManager>();
	colliderMgr->RegisterCollider("Player", rect->GetCollider());
	colliderMgr->RegisterCollider("Monster", rect1->GetCollider());

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

	if (input->KeyPress(VK_RIGHT)) {
		Vector3 pos = rect->GetTransform()->GetPosition();
		pos.x += 10.0f;
		rect->GetTransform()->SetPosition(pos);
	}

	if (input->BtnDown(0)) { //마우스 충돌
		Vector2 mousePos = input->GetMousePosition();
		Vector3 mouseWorld = camera->ScreenToWorldPoint(mousePos);

		auto effectMgr = context->GetSubsystem<EffectManager>();
		effectMgr->AddEffect("metalslug.png", "Idle.xml", mouseWorld);

		//auto transform = rect->GetTransform();

		//bool bCheck = true;
		//bCheck &= (mouseWorld.x > transform->GetPosition().x - 14.0f);
		//bCheck &= (mouseWorld.x < transform->GetPosition().x + 14.0f);
		//bCheck &= (mouseWorld.x > transform->GetPosition().x - 19.0f);
		//bCheck &= (mouseWorld.x < transform->GetPosition().x + 19.0f);

		//if (bCheck) 
		//	int a = 0;
	}

	rect->Update();
	rect1->Update();

	////////////////////////////////////////////////////////////////

	auto colliderMgr = context->GetSubsystem <ColliderManager>();
	colliderMgr->HitCheck_AABB("Player", "Monster");
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
