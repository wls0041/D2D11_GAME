#include "stdafx.h"
#include "SceneManager.h"
#include "../../Scene/Scene.h"
#include "Framework/Scene/Component/Transform.h"
#include "Framework/Scene/Component/AudioSource.h"
#include "Framework/Rendering/Rect.h"
#include "Framework/Scene/Component/Camera.h"

SceneManager::SceneManager(Context * context) : ISubsystem(context), currentScene(nullptr), delayTime(0), life(5), bLose(false), bWin(false), bStart(false)
{
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	winMessage = new Rect(context, "Message_Win.png");
	winMessage->GetTransform()->SetScale({ 200.0f * 4.5f, 200.0f, 1 });
	winMessage->GetTransform()->SetPosition({ 0, 25, 0 });
	winMessage->Update();

	loseMessage = new Rect(context, "Message_Lose.png");
	loseMessage->GetTransform()->SetScale({ 200.0f * 5.0f * 0.5f, 200.0f, 1 });
	loseMessage->GetTransform()->SetPosition({ 0, 25, 0 });
	loseMessage->Update();

	title = new Rect(context, "Message_Title.png");
	title->GetTransform()->SetScale({ 300.0f * 5.5f * 0.5f, 300.0f, 1 });
	title->GetTransform()->SetPosition({ 0, 25, 0 });
	title->Update();

	bgm = new AudioSource(context);
	bgm->SetAudioClip("Pang_Title.wav");

	winBgm = new AudioSource(context);
	winBgm->SetAudioClip("Pang_Win.wav");

	loseBgm = new AudioSource(context);
	loseBgm->SetAudioClip("Pang_Lose.wav");
}

SceneManager::~SceneManager()
{
	//Æí¹ý
	for (auto scene : scenes) SAFE_DELETE(scene.second);

	SAFE_DELETE(loseBgm);
	SAFE_DELETE(winBgm);
	SAFE_DELETE(bgm);
	SAFE_DELETE(title);
	SAFE_DELETE(winMessage);
	SAFE_DELETE(loseMessage);
}

void SceneManager::Initialize()
{
	RegisterScene("First", new Scene(context, life));
	SetCurrentScene("First");
}

void SceneManager::Update()
{
	auto input = context->GetSubsystem<Input>();
	if (input->KeyDown(VK_SPACE) && !bStart) {
		bStart = true;
		bgm->Stop();
		SAFE_DELETE(cameraBuffer);
		SAFE_DELETE(camera);
	}
	if (!bStart) {
		camera->Update();
		auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
		cameraData->View = camera->GetViewMatrix();
		cameraData->Projection = camera->GetProjectionMatrix();
		cameraBuffer->Unmap();

		bgm->Play();
	}

	if (bStart) {
		if (input->KeyDown(VK_ESCAPE)) PostQuitMessage(0);

		if (life > 0 && !bWin && !bLose) {
			if (currentScene) {
				currentScene->Update();

				if (currentScene->IsClear()) {
					SAFE_DELETE(currentScene);
					scenes.erase("First");
					bWin = true;
					winBgm->Play();
				}
				else if (currentScene->IsDead()) {
					life = currentScene->GetLife();
					SAFE_DELETE(currentScene);
					scenes.erase("First");
				}
			}
			else delayTime++;

			if (delayTime > 30) {
				RegisterScene("First", new Scene(context, life));
				SetCurrentScene("First");
				delayTime = 0;
			}
		}
		if (life <= 0) {
			if(!bLose) loseBgm->Play();
			bLose = true;
		}
	}
}

void SceneManager::Render()
{
	if (!bStart) {
		cameraBuffer->BindPipeline(ShaderType::VS, 0);

		auto dw = context->GetSubsystem<DirectWrite>();
		dw->Text_Middle(L"<< Press Space >>", Vector2(670, 650), 40.0f, Color(1, 1, 1, 1));

		title->Render(); 
	}
	if (bStart) {
		if (bWin) winMessage->Render();
		else if (bLose) loseMessage->Render();

		if (currentScene) currentScene->Render();
	}
}

void SceneManager::SetCurrentScene(const string & name)
{
	assert(scenes.count(name) > 0);
	currentScene = scenes[name];
}

void SceneManager::RegisterScene(const string & name, Scene * scene)
{
	assert(scenes.count(name) < 1);
	scene->SetName(name);
	//1
	//scenes[name] = scene;
	
	//2
	scenes.insert(make_pair(name, scene));
}
