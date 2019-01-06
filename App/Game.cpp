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
		//�־��� y, z���� ���� CROSS(����)������ �Ͽ� x���� �˾Ƴ�, ���� ���� ����� �־��� ������ ��������(A X B��� �޼����� a���� b�� ��������� �� ���� ����)
		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, ��  
	}

	//Create View Matrix
	{
		D3DXMatrixOrthoLH(&projection, 1280, 720, 0, 1);  //������ ������ ������ ����, ī�޶� �� - zn(near plane), ������ �� - zf(far plane)
		//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1);
	}
	//������� ������ ��ȯ��Ű�� ���� VS. ���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)

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

	auto data = static_cast<CameraData*>(cameraBuffer->Map()); //������ ��Ʈ�� ������. ������ �����Ƿ� cameraBuffer�� ĳ����

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
	//ȭ�� ������ ������ ���� �� �����
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
		if (rect[i]->GetPosition().x > -400 && rect[i]->GetPosition().x < -200) { //��� ���̱�
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
				if (IsCollision_CirclePoint(rect_left, rect_top)) IsEnd = true; //�»��
				if (IsCollision_CirclePoint(rect_right, rect_top)) IsEnd = true; //����
				if (IsCollision_CirclePoint(rect_left, rect_bottom)) IsEnd = true; //���ϴ�
				if (IsCollision_CirclePoint(rect_right, rect_bottom)) IsEnd = true; //���ϴ�
			}
		}
	}

	if (IsEnd) MessageBoxA(NULL, "!! ��ֹ��� �浹 !!", "~~~~ THE END ~~~~", MB_OK);
}

bool Game::IsCollision_CirclePoint(float point_X, float point_Y)
{
	float deltaX = player->GetPosition().x - point_X;
	float deltaY = player->GetPosition().y - point_Y;
	float length = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (length > player->GetScale().x) return false;
	return true;
}
