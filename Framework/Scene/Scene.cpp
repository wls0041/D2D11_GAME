#include "stdafx.h"
#include "Scene.h"
#include "./Component/Camera.h"
#include "../Rendering/Rect.h"
#include "../Rendering/Anim.h"
#include "../Resource/AudioClip.h"

Scene::Scene(class Context *context) : context(context), GameStart(false), bJump(false), bReady(false), bEnd(false),
	score(0), jumpTimer(0.0f), hitTimer(0.0f), birdPosition(-200.0f, 0.0f), numberScale(0, 0), moveTitle(2.0f), titleTimer(0.0f)
{
	input = context->GetSubsystem<Input>();
	camera = new Camera(context);
	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	boardScale = { 720.0f, 720.0f };
	backPosition[0] = { 0.0f, 0.0f };
	backPosition[1] = { boardScale.x, 0.0f };
	
	numberScale = { 14, 20 };

	for (int i = 0; i < 2; i++) {
		ground[i] = new Anim(context);
		ground[i]->SetScale({ boardScale.x, boardScale.y * 0.2f, 1 });
		ground[i]->SetPosition({ boardScale.x * i, -360 + boardScale.y * 0.07f, 0 });
		ground[i]->SetOffset({ 148, 0 });
		ground[i]->SetSize({ 148.0f, 55.0f });
		ground[i]->Update();

		back[i] = new Anim(context);
		back[i]->SetScale({ boardScale.x, boardScale.y, 1 });
		back[i]->SetPosition({ boardScale.x * i, 0, 0 });
		back[i]->SetOffset({ 0, 0 });
		back[i]->SetSize({ 142.0f, 256.0f });
		back[i]->Update();
	}

	for (int i = 0; i < 5; i++) {
		pipePosition_top[i] = { boardScale.x / 3 * (i + 4), 360};

		pipe_top[i] = new Anim(context);
		pipe_top[i]->SetScale({ 55, 360, 1 });
		pipe_top[i]->SetPosition({ pipePosition_top[i].x,  pipePosition_top[i].y - rand() % 4 * 40 , 0 });
		pipe_top[i]->SetOffset({ 302, 14 });
		pipe_top[i]->SetSize({ 26.0f, 121.0f });
		pipe_top[i]->Update();

		pipe_bot[i] = new Anim(context);
		pipe_bot[i]->SetScale({ 55, 360, 1 });
		pipe_bot[i]->SetPosition({ pipePosition_top[i].x, -pipePosition_top[i].y + rand() % 5 * 40 , 0 });
		pipe_bot[i]->SetOffset({ 330, 0 });
		pipe_bot[i]->SetSize({ 26.0f, 121.0f });
		pipe_bot[i]->Update();

		pipePosition_top[i].x = pipe_top[i]->GetPosition().x;
		pipePosition_top[i].y = pipe_top[i]->GetPosition().y;
		pipePosition_bot[i].x = pipe_bot[i]->GetPosition().x;
		pipePosition_bot[i].y = pipe_bot[i]->GetPosition().y;
	}
	D3DXVECTOR2 offset[10] = { {288,100},{291,118}, {289,134}, {289,150}, {287,173},
							  {287,185}, {165,245}, {175,245}, {185,245}, {195,245} };

	for (int i = 0; i < 10; i++) {
		number[i] = new Anim(context);
		number[i]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[i]->SetPosition({ 0, 0, 0 });
		number[i]->SetOffset(offset[i]);

		if (i == 1) number[i]->SetSize({ 5, 10 });
		else number[i]->SetSize({ 7, 10 });
	}

	message[0] = new Anim(context); //타이틀
	message[0]->SetScale({ 280, 70, 1 });
	message[0]->SetPosition({ -25, 100, 0 });
	message[0]->SetOffset({ 147, 174 });
	message[0]->SetSize({ 94, 20 });

	message[1] = new Anim(context); //레디
	message[1]->SetScale({ 200, 50, 1 });
	message[1]->SetPosition({ 0, 170, 0 });
	message[1]->SetOffset({ 147, 222 });
	message[1]->SetSize({ 85, 20 });

	message[2] = new Anim(context); //게임 오버
	message[2]->SetScale({ 240, 60, 1 });
	message[2]->SetPosition({ 0, 200, 0 });
	message[2]->SetOffset({ 147, 200 });
	message[2]->SetSize({ 92, 17 });

	message[0]->Update();
	message[1]->Update();
	message[2]->Update();

	board = new Anim(context);
	board->SetScale({ 300, 150, 1 });
	board->SetPosition({ 0, 50, 0 });
	board->SetOffset({ 146, 58 });
	board->SetSize({ 113, 58 });
	board->Update();

	bird = new Rect(context);
	bird->SetScale({ 2.5f, 2.5f, 1 });
	bird->SetPosition({ -200, 100, 0 });
	bird->SaveCurPosition(0, 0);
	bird->Update();

	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("flap_back.mp3");
	clip->Play();
}

Scene::~Scene()
{
	for (auto audioClip : clips) SAFE_DELETE(audioClip);
	SAFE_DELETE(bird);	
	for (int i = 0; i < 10; i++) SAFE_DELETE(number[i])
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
	/////////////////////오디오 복사 생성자 테스트코드//////////////////////////	
	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	auto clip = resourceMgr->Load<AudioClip>("flap_back.mp3");

	if (input->KeyDown('Q')) clip->Play();
	else if (input->KeyDown('W')) clip->Pause();
	else if (input->KeyDown('E')) clip->Stop();

	camera->Update();
	auto cameraData = static_cast<CameraData*>(cameraBuffer->Map());
	D3DXMatrixTranspose(&cameraData->View, &camera->GetViewMatrix()); //쉐이더는 열우선이므로 전치 필요
	D3DXMatrixTranspose(&cameraData->Projection, &camera->GetProjectionMatrix());
	cameraBuffer->Unmap();

	if (!GameStart) {
		if (input->KeyPress(VK_SPACE)) {
			GameStart = true;
			bird->SetPosition({ -200, 0, 0 });
			bird->Update();
		}
		if (titleTimer > 30.0f) {
			titleTimer = 0.0f;
			moveTitle = -moveTitle;
		}
		else titleTimer += 1.0f;
		message[0]->SetPosition({ message[0]->GetPosition().x, message[0]->GetPosition().y + moveTitle, 0 });
		bird->SetPosition({ 170, bird->GetPosition().y + moveTitle, 0 });
		message[0]->Update(); bird->Update();
	}
	if (GameStart) {
		if (input->KeyDown(VK_SPACE)) bReady = true;
		if (bReady) {
			if (input->KeyDown(VK_SPACE)) {
				static auto wing = resourceMgr->Load<AudioClip>("flap_wing.wav");
				clips.push_back(new AudioClip(*wing));
				clips.back()->Play();
				bJump = true;
				jumpTimer = 0.0f;
			}

			if (!bJump) {
				birdPosition -= D3DXVECTOR2(0.0f, 4.0f);
				bird->SaveCurPosition_y(-4.0f);
			}
			else {
				jumpTimer += 1.0f;
				birdPosition.y += 3.0f;
				bird->SaveCurPosition_y(3.0f);
			}

			if (jumpTimer > 20) {
				bJump = false;
				jumpTimer = 0.0f;
			}

			birdPosition.y = Math::clamp(birdPosition.y, -360 + boardScale.y * 0.17f, boardScale.y * 0.5f - 12 * 2.5f / 2);
			bird->SetPosition({ birdPosition.x, birdPosition.y, 0 });
			if (bEnd) {
				if (hitTimer < 22) hitTimer += 1.0f;
				bird->SaveCurPosition(0, -4.0f);
				bird->Pause();

				if (hitTimer == 20) {
					static auto die = resourceMgr->Load<AudioClip>("flap_die.wav");
					clips.push_back(new AudioClip(*die));
					clips.back()->Play();
				}
			}
			else bird->SaveCurPosition_x(5.0f);
			bird->Update();

			if (!bEnd) {
				for (int i = 0; i < 2; i++) {
					backPosition[i].x = back[i]->GetPosition().x - 5.0f;

					back[i]->SetPosition({ backPosition[i].x, back[i]->GetPosition().y, 0 });
					back[i]->Update();

					ground[i]->SetPosition({ backPosition[i].x, ground[i]->GetPosition().y, 0 });
					ground[i]->Update();

					IsBoardOutBack({ backPosition[i].x, backPosition[i].y, 0 }, i);
				}

				for (int i = 0; i < 5; i++) {
					if (IsCollision(birdPosition, pipe_top[i]->GetPosition())) GameEnd();
					if (IsCollision(birdPosition, pipe_bot[i]->GetPosition())) GameEnd();
					GetScore(pipe_top[i]->GetPosition().x);

					pipePosition_top[i].x = pipe_top[i]->GetPosition().x - 5.0f;
					pipePosition_bot[i].x = pipe_top[i]->GetPosition().x - 5.0f;

					pipe_top[i]->SetPosition({ pipePosition_top[i].x, pipe_top[i]->GetPosition().y, 0 });
					pipe_top[i]->Update();

					pipe_bot[i]->SetPosition({ pipePosition_bot[i].x, pipe_bot[i]->GetPosition().y, 0 });
					pipe_bot[i]->Update();

					IsBoardOutPipe({ pipePosition_top[i].x, pipePosition_top[i].y, 0 }, i);
				}

				if (birdPosition.y - 12 * 2.5f / 2 + 3 <= -360 + boardScale.y * 0.17f) GameEnd();
			}
		}
		else {//별도의 처리
			bird->SetPosition({ -200, 0, 0 });
			bird->Update();
		}
	}
}

void Scene::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);

	if (!GameStart) message[0]->Render();//시작 화면
	if (GameStart && !bReady) message[1]->Render(); //ready출력
	if (GameStart && !bEnd) RenderPlayScore();//게임 중 점수 출력
	if (bEnd) { //끝난 후 점수판 출력
		message[2]->Render();
		RenderPlayScore(10.0f, 20.0f, 90.0f, -220.0f);
		board->Render();
	}
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
		ground[index]->SetScale({ boardScale.x, boardScale.y * 0.2f, 1 });
		ground[index]->SetPosition({ boardScale.x - 10.0f, -360 + boardScale.y * 0.07f, 0 });
		ground[index]->SetOffset({ 148, 0 });
		ground[index]->SetSize({ 148.0f, 55.0f });

		back[index] = new Anim(context);
		back[index]->SetScale({ boardScale.x, boardScale.y, 1 });
		back[index]->SetPosition({ boardScale.x -10.0f, 0, 0 });
		back[index]->SetOffset({ 0, 0 });
		back[index]->SetSize({ 142.0f, 256.0f });
	}
}

void Scene::IsBoardOutPipe(D3DXVECTOR3 position, int index)
{
	if (position.x + 55.0f * 0.5f < -boardScale.x / 2.0f) {
		SAFE_DELETE(pipe_top[index]);
		SAFE_DELETE(pipe_bot[index]);

		pipePosition_top[index] = { pipePosition_top[(index + 1) % 5].x + boardScale.x / 3 * 4, 360};

		pipe_top[index] = new Anim(context);
		pipe_top[index]->SetScale({ 55, 360, 1 });
		pipe_top[index]->SetPosition({ pipePosition_top[index].x,  pipePosition_top[index].y - rand() % 4 * 40 , 0 });
		pipe_top[index]->SetOffset({ 302, 14 });
		pipe_top[index]->SetSize({ 26.0f, 121.0f });

		pipe_bot[index] = new Anim(context);
		pipe_bot[index]->SetScale({ 55, 360, 1 });
		pipe_bot[index]->SetPosition({ pipePosition_top[index].x, -pipePosition_top[index].y + rand() % 5 * 40 , 0 });
		pipe_bot[index]->SetOffset({ 330, 0 });
		pipe_bot[index]->SetSize({ 26.0f, 121.0f });

		pipePosition_top[index].x = pipe_top[index]->GetPosition().x;
		pipePosition_top[index].y = pipe_top[index]->GetPosition().y;
		pipePosition_bot[index].x = pipe_bot[index]->GetPosition().x;
		pipePosition_bot[index].y = pipe_bot[index]->GetPosition().y;
	}
}

bool Scene::IsCollision(D3DXVECTOR2 first, D3DXVECTOR3 second)
{
	bool bVertical = false;
	bool bHorizon = false;
	D3DXVECTOR2 scale = { 55 / 2.0f, 360 / 2.0f };
	float dis1_x = 17.0f * 2.5f / 2.0f, dis1_y = 12.0f * 2.5f / 2.0f;
	float l_right = first.x + dis1_x, l_left = first.x - dis1_x, l_bottom = first.y - dis1_y, l_top = first.y + dis1_y;
	float r_right = second.x + scale.x, r_left = second.x - scale.x, r_bottom = second.y - scale.y, r_top = second.y + scale.y;

	if (l_left <= r_right && l_right >= r_left) bVertical = true;
	if (l_top >= r_bottom && l_bottom <= r_top) bHorizon = true;
	if (bVertical && bHorizon) return true;

	return false;
}			

void Scene::GameEnd()
{
	auto resourceMgr = context->GetSubsystem<ResourceManager>();
	static auto hit = resourceMgr->Load<AudioClip>("flap_hit.wav");
	clips.push_back(new AudioClip(*hit));
	clips.back()->Play();

	bEnd = true;
}

void Scene::RenderPlayScore(float width, float height, float x, float y)
{
	numberScale = { width, height };
	D3DXVECTOR2 numberPos{ x, y };

	int curScore = score;
	int one = -1, ten = -1, hund = -1;
	if (curScore / 100 > 0) { hund = curScore / 100; curScore %= hund; }
	if (curScore / 10 > 0) { ten = curScore / 10; curScore %= ten; }
	one = curScore;

	if (hund > 0) {
		number[hund]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[hund]->SetPosition({ numberScale.x * 1.05f + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });

		number[ten]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[ten]->SetPosition({ 0 + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });

		number[one]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[one]->SetPosition({ -numberScale.x * 1.05f + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });

		number[hund]->Update();
		number[ten]->Update();
		number[one]->Update();

		number[hund]->Render();
		number[ten]->Render();
		number[one]->Render();
	}
	else if (ten > 0) {
		number[ten]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[ten]->SetPosition({ -numberScale.x * 0.55f + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });

		number[one]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[one]->SetPosition({ numberScale.x * 0.55f + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });

		number[ten]->Update();
		number[one]->Update();

		number[ten]->Render();
		number[one]->Render();
	}
	else {
		number[one]->SetScale({ numberScale.x, numberScale.y, 0 });
		number[one]->SetPosition({ 0 + numberPos.x, boardScale.y * 0.4f + numberPos.y, 0 });
		number[one]->Update();
		number[one]->Render();
	}
}

void Scene::GetScore(float position)
{
	if (position == -200) {
		score++;

		auto resourceMgr = context->GetSubsystem<ResourceManager>();
		static auto point = resourceMgr->Load<AudioClip>("flap_point.wav");
		clips.push_back(new AudioClip(*point));
		clips.back()->Play();
	}
}