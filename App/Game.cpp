#include "stdafx.h"
#include "Game.h"
#include "./Framework/Rendering/Rect.h"
#include "./Framework/Rendering/Player.h"

void Game::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();

	cameraBuffer = new ConstantBuffer(context);
	cameraBuffer->Create<CameraData>();

	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	//Create projection Matrix
	{
		//주어진 y, z축을 토대로 CROSS(외적)연산을 하여 x축을 알아냄, 외적 연산 결과는 주어진 벡터의 수직벡터(A X B라면 왼손으로 a에서 b로 감아쥐었을 때 엄지 방향)
		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); //반환 받을 변수, 카메라 위치, 바라보는 방향, 위  
	}

	//Create View Matrix
	{
		D3DXMatrixOrthoLH(&projection, 1280, 720, 0, 1);  //투영될 공간의 사이즈 지정, 카메라 앞 - zn(near plane), 투영된 곳 - zf(far plane)
		//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1);
	}
	//만들어진 공간을 변환시키는 것은 VS. 따라서 VS로 가서 만듬(color.hlsld의 cbuffer와 vs), 보낼 때 정보를 보내줘야함(id3d11buffer)

	player = new Player(context);
	player->SetScale(D3DXVECTOR3(60, 60, 1)); 
	player->Update();

	for (int i = 0; i < 6; i++) {
		D3DXVECTOR3 randPosition;
		float randY = (rand() % 7) * 37 + 200;

		randPosition.x = 1280 / 5 * (i + 3) - 640;
		randPosition.y = randY / 2 - 360;
		randPosition.z = 0;

		rect[i] = new Rect(context);
		rect[i]->SetScale(D3DXVECTOR3(80, randY, 1));
		rect[i]->SetPosition(randPosition);
		rect[i]->Update();
	}
}

void Game::Update()
{
	timer->Update();
	input->Update();

	auto data = static_cast<CameraData*>(cameraBuffer->Map()); //꼬봉의 노트를 내보냄. 형식이 없으므로 cameraBuffer로 캐스팅

	D3DXMatrixTranspose(&data->View, &view);
	D3DXMatrixTranspose(&data->Projection, &projection);

	cameraBuffer->Unmap();

	if (input->KeyPress(VK_SPACE)) GameStart = true;
	if (GameStart) {
		D3DXVECTOR3 position[6];
		for (int i = 0; i < 6; i++) position[i] = rect[i]->GetPosition();

		D3DXVECTOR3 position_p = player->GetPosition();
		float jumpTimer = player->GetJumpTime();

		if (input->KeyDown(VK_SPACE)) player->SetJump(true);

		if (!player->GetJump()) position_p.y -= 300.0f * timer->GetDeltaTimeSec();
		else {
			jumpTimer += 0.1f;
			position_p.y += 400.0f * timer->GetDeltaTimeSec();
			player->SetJumpTime(jumpTimer);
		}
		if (jumpTimer > 60) {
			player->SetJump(false);
			player->SetJumpTime(0.0f);
		}

		player->SetPosition(position_p);
		player->Update();

		for (int i = 0; i < 6; i++) {
			position[i].x -= 300.0f * timer->GetDeltaTimeSec();
			rect[i]->SetPosition(position[i]);
			rect[i]->Update();

			IsBoardOut(position[i], i);
		}

		IsCollision_CircleRect();
	}
}

void Game::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	for (int i = 0; i < 6; i++) rect[i]->Render();
	player->Render();
}

void Game::Destroy()
{
	SAFE_DELETE(player);
	for (int i = 0; i < 6; i++) SAFE_DELETE(rect[i]);
	SAFE_DELETE(cameraBuffer);
}

void Game::IsBoardOut(D3DXVECTOR3 position, int index)
{
	//화면 밖으로 나가면 삭제 후 재생성
	if (position.x + 40.0f < -640.0f) {
		float randNum = (rand() % 7) * 25 + 200;
		SAFE_DELETE(rect[index]);

		rect[index] = new Rect(context);
		rect[index]->SetScale(D3DXVECTOR3(80, randNum, 1));
		rect[index]->SetPosition(D3DXVECTOR3(1280 / 5 * 6 - 640, randNum / 2 - 360, 0));
	}
}

void Game::IsCollision_CircleRect()
{
	float circle_X, circle_Y, circle_radius;
	circle_X = player->GetPosition().x;
	circle_Y = player->GetPosition().y;
	circle_radius = player->GetScale().x;

	bool IsEnd = false;

	for (int i = 0; i < 6; i++) {
		if (rect[i]->GetPosition().x > -400 && rect[i]->GetPosition().x < -200) { //계산 줄이기
			float rect_left, rect_right, rect_top, rect_bottom;

			rect_left = rect[i]->GetPosition().x - 40;
			rect_right = rect[i]->GetPosition().x + 40;
			rect_top = rect[i]->GetPosition().y + (rect[i]->GetPosition().y + 360);
			rect_bottom = rect[i]->GetPosition().y - (rect[i]->GetPosition().y + 360);

			if ((rect_left <= circle_X && circle_X <= rect_right) || (rect_top >= circle_Y && circle_Y >= rect_bottom)) {
				float resize_left = rect_left - circle_radius;
				float resize_top = rect_top + circle_radius;
				float resize_right = rect_right + circle_radius;
				float resize_bottom = rect_bottom - circle_radius;

				if ((resize_left < circle_X && circle_X < resize_right) && (resize_top > circle_Y && circle_Y > resize_bottom)) IsEnd = true;
			}
			else {
				if (IsCollision_CirclePoint(rect_left, rect_top)) IsEnd = true; //좌상단
				if (IsCollision_CirclePoint(rect_right, rect_top)) IsEnd = true; //우상단
				if (IsCollision_CirclePoint(rect_left, rect_bottom)) IsEnd = true; //좌하단
				if (IsCollision_CirclePoint(rect_right, rect_bottom)) IsEnd = true; //우하단
			}
		}
	}

	if (IsEnd) MessageBoxA(NULL, "!! 장애물에 충돌 !!", "~~~~ THE END ~~~~", MB_OK);
}

bool Game::IsCollision_CirclePoint(float point_X, float point_Y)
{
	float deltaX = player->GetPosition().x - point_X;
	float deltaY = player->GetPosition().y - point_Y;
	float length = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (length > player->GetScale().x) return false;
	return true;
}
