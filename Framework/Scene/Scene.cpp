#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "../Rendering/Rect.h"
#include "../Rendering/Anim.h"
#include "../Resource/AudioClip.h"

Scene::Scene(class Context *context) : context(context), GameStart(false), bJump(false), jumpTimer(0.0f), birdPosition(-200.0f, 0.0f)
{
	input = context->GetSubsystem<Input>();
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	boardScale = { 720.0f, 720.0f / 256.0f };
	backPosition[0] = { 0.0f, 0.0f };
	backPosition[1] = { boardScale.x, 0.0f };

	for (int i = 0; i < 2; i++) {
		ground[i] = new Anim(context);
		ground[i]->SetScale({ boardScale.x / 148.0f, boardScale.y, 1 });
		ground[i]->SetPosition({ boardScale.x * i, -285, 0 });
		ground[i]->SetOffset({ 148, 0 });
		ground[i]->SetSize({ 148.0f, 55.0f });
		ground[i]->Update();

		back[i] = new Anim(context);
		back[i]->SetScale({ boardScale.x / 142.0f, boardScale.y, 1 });
		back[i]->SetPosition({ boardScale.x * i, 0, 0 });
		back[i]->SetOffset({ 0, 0 });
		back[i]->SetSize({ 142.0f, 256.0f });
		back[i]->Update();
	}

	for (int i = 0; i < 5; i++) {
		pipePosition_top[i] = { boardScale.x + boardScale.x / 3 * (i + 1), 720 / 2 -  121.0f * 3.0f / 2.0f + 150.0f};

		pipe_top[i] = new Anim(context);
		pipe_top[i]->SetScale({ 2, 3, 1 });
		pipe_top[i]->SetPosition({ pipePosition_top[i].x,  pipePosition_top[i].y + rand() % 5 * 40 , 0 });
		pipe_top[i]->SetOffset({ 302, 14 });
		pipe_top[i]->SetSize({ 26.0f, 121.0f });
		pipe_top[i]->Update();

		pipe_bot[i] = new Anim(context);
		pipe_bot[i]->SetScale({ 2, 3, 1 });
		pipe_bot[i]->SetPosition({ pipePosition_top[i].x, -pipePosition_top[i].y + rand() % 5 * 40 , 0 });
		pipe_bot[i]->SetOffset({ 330, 0 });
		pipe_bot[i]->SetSize({ 26.0f, 121.0f });
		pipe_bot[i]->Update();

		pipePosition_top[i].x = pipe_top[i]->GetPosition().x;
		pipePosition_top[i].y = pipe_top[i]->GetPosition().y;
		pipePosition_bot[i].x = pipe_bot[i]->GetPosition().x;
		pipePosition_bot[i].y = pipe_bot[i]->GetPosition().y;
	}

	bird = new Rect(context);
	bird->SetScale({ 2.5f, 2.5f, 1 });
	bird->SetPosition({ -200, 0, 0 });
	bird->Update();

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("Stage1.mp3");
	clip->Play();
}

Scene::~Scene()
{
	SAFE_DELETE(bird);
	for (int i = 0; i < 5; i++) {
		SAFE_DELETE(pipe_top[i]);
		SAFE_DELETE(pipe_bot[i]);
	}
	for (int i = 0; i < 2; i++) {
		SAFE_DELETE(back[i]);
		SAFE_DELETE(ground[i]);
	}
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(camera);
}

void Scene::Update()
{
	input->Update();
	camera->Update();
	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	D3DXMatrixTranspose(&cameraData->View, &camera->GetViewMatrix()); //쉐이더는 열우선이므로 전치 필요
	D3DXMatrixTranspose(&cameraData->Projection, &camera->GetProjectionMatrix());
	cameraBuffer->Unmap();
	
	if (!GameStart) {
		if (input->KeyPress(VK_SPACE)) GameStart = true;
	}
	if (GameStart) {
		if (input->KeyDown(VK_SPACE)) {
			bJump = true;
			jumpTimer = 0.0f;
		}

		if (!bJump) birdPosition -= D3DXVECTOR2(0.0f, 4.0f);
		else {
			jumpTimer += 1.0f;
			birdPosition.y += 3.0f;
		}

		birdPosition.y = Math::clamp(birdPosition.y, boardScale.y * 55.0f - 350, 360.0f - 10);

		if (jumpTimer > 20) {
			bJump = false;
			jumpTimer = 0.0f;
		}

		for (int i = 0; i < 2; i++) {
			backPosition[i].x = back[i]->GetPosition().x - 5.0f;

			back[i]->SetPosition({ backPosition[i].x, back[i]->GetPosition().y, 0 });
			back[i]->Update();

			ground[i]->SetPosition({ backPosition[i].x, ground[i]->GetPosition().y, 0 });
			ground[i]->Update();

			IsBoardOutBack({ backPosition[i].x, backPosition[i].y, 0 }, i);
		}

		for (int i = 0; i < 5; i++) {
			if (IsCollision(birdPosition, pipe_top[i]->GetPosition(), pipe_top[i]->GetReSize())) GameEnd();
			if (IsCollision(birdPosition, pipe_bot[i]->GetPosition(), pipe_bot[i]->GetReSize())) GameEnd();

			pipePosition_top[i].x = pipe_top[i]->GetPosition().x - 5.0f;
			pipePosition_bot[i].x = pipe_top[i]->GetPosition().x - 5.0f;

			pipe_top[i]->SetPosition({ pipePosition_top[i].x, pipe_top[i]->GetPosition().y, 0 });
			pipe_top[i]->Update();

			pipe_bot[i]->SetPosition({ pipePosition_bot[i].x, pipe_bot[i]->GetPosition().y, 0 });
			pipe_bot[i]->Update();

			IsBoardOutPipe({ pipePosition_top[i].x, pipePosition_top[i].y, 0 }, i);
		}

		if (birdPosition.y <= boardScale.y * 55.0f - 350.0f) GameEnd();
		bird->SetPosition({ birdPosition.x, birdPosition.y, 0 });
		bird->Update();
	}
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);

	bird->Render();
	for (int i = 0; i < 2; i++) ground[i]->Render();
	for (int i = 0; i < 5; i++) {
		pipe_top[i]->Render();
		pipe_bot[i]->Render();
	}
	for (int i = 0; i < 2; i++) back[i]->Render();

}

void Scene::IsBoardOutBack(D3DXVECTOR3 position, int index)
{
	//화면 밖으로 나가면 삭제 후 재생성
	if (position.x + boardScale.x / 2.0f < -boardScale.x / 2.0f) {
		SAFE_DELETE(ground[index]);
		SAFE_DELETE(back[index]);

		backPosition[index].x = boardScale.x;

		ground[index] = new Anim(context);
		ground[index]->SetScale({ boardScale.x / 148.0f, boardScale.y, 1 });
		ground[index]->SetPosition({ boardScale.x - 10.0f, -285, 0 });
		ground[index]->SetOffset({ 148, 0 });
		ground[index]->SetSize({ 148.0f, 55.0f });

		back[index] = new Anim(context);
		back[index]->SetScale({ boardScale.x / 142.0f, boardScale.y, 1 });
		back[index]->SetPosition({ boardScale.x -10.0f, 0, 0 });
		back[index]->SetOffset({ 0, 0 });
		back[index]->SetSize({ 142.0f, 256.0f });
	}
}

void Scene::IsBoardOutPipe(D3DXVECTOR3 position, int index)
{
	if (position.x + 52.0f / 2.0f < -boardScale.x / 2.0f) {
		SAFE_DELETE(pipe_top[index]);
		SAFE_DELETE(pipe_bot[index]);

		pipePosition_top[index] = { boardScale.x / 3 * 5 - 360, 720 / 2 - 121.0f * 3.0f / 2.0f + 150.0f };

		pipe_top[index] = new Anim(context);
		pipe_top[index]->SetScale({ 2, 3, 1 });
		pipe_top[index]->SetPosition({ pipePosition_top[index].x,  pipePosition_top[index].y + rand() % 5 * 40 , 0 });
		pipe_top[index]->SetOffset({ 302, 14 });
		pipe_top[index]->SetSize({ 26.0f, 121.0f });

		pipe_bot[index] = new Anim(context);
		pipe_bot[index]->SetScale({ 2, 3, 1 });
		pipe_bot[index]->SetPosition({ pipePosition_top[index].x,  -pipePosition_top[index].y + rand() % 5 * 40 , 0 });
		pipe_bot[index]->SetOffset({ 330, 0 });
		pipe_bot[index]->SetSize({ 26.0f, 121.0f });

		pipePosition_top[index].x = pipe_top[index]->GetPosition().x;
		pipePosition_top[index].y = pipe_top[index]->GetPosition().y;
		pipePosition_bot[index].x = pipe_bot[index]->GetPosition().x;
		pipePosition_bot[index].y = pipe_bot[index]->GetPosition().y;
	}
}

bool Scene::IsCollision(D3DXVECTOR2 first, D3DXVECTOR3 second, D3DXVECTOR2 scale)
{
	bool bVertical = false;
	bool bHorizon = false;
	float dis1_x = 17.0f * 2.5f / 2.0f, dis1_y = 12.0f * 2.5f / 2.0f;
	float l_right = first.x + dis1_x, l_left = first.x - dis1_x, l_bottom = first.y - dis1_y, l_top = first.y + dis1_y;
	float r_right = second.x + scale.x, r_left = second.x - scale.x, r_bottom = second.y - scale.y, r_top = second.y + scale.y;

	if (l_left <= r_right && l_right >= r_left) { 
		bVertical = true;
  	}if (l_top >= r_bottom && l_bottom <= r_top) {
		bHorizon = true;
	}

	if (bVertical && bHorizon) return true;
	return false;
}			

void Scene::GameEnd()
{
	MessageBoxA(NULL, "!! 장애물에 충돌 !!", "~~~~ THE END ~~~~", MB_OK);
	PostQuitMessage(0);
}