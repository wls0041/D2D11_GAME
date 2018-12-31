#include "stdafx.h"
#include "Game.h"
#include "./Rendering/Rect.h"
#include "./Rendering/Player.h"

void Game::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();

	//Create Constant Buffer
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //ī�޶� �����̶� �����̸� �ǽð����� �ݿ��Ǿ�� ��
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.ByteWidth = sizeof(Data);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //usage�� dynamic�� �� �߰�. cpu�� ��������� �����϶�(����� gpu�� �� ���ֹǷ�)

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cbuffer);
		assert(SUCCEEDED(hr));
	}

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
	for (int i = 0; i < 10; i++){
		rect[i] = new Rect(context);
	}

}

void Game::Update()
{
	if (player->GetScale() > 450) MessageBoxA(NULL, "�÷��̾��� ũ�Ⱑ �Ѱ迡 ����!!", "~~~~ THE END ~~~~", MB_OK);
	IsCollision();

	D3DXMatrixTranspose(&data.View, &view);
	D3DXMatrixTranspose(&data.Projection, &projection);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//��, ���. ��->�� �̻� ������ ���ϵ��� ����(update �߿� cbuffer�� �� �ǵ��)(�������� ����)->subResource�� ��� ���� �����ͼ� �־���,  ���->���� ���� ǰ. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource�� �����ض�, ������ ������(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //����, ����, ������ ũ��
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);

	player->Update();
	for (int i = 0; i < 10; i++) {
		rect[i]->Update();
	}
}

void Game::Render()
{
	auto dc = graphics->GetDeviceContext();
	dc->VSSetConstantBuffers(0, 1, &cbuffer); //cbuffer�Ѱ���

	player->Render();
	for (int i = 0; i < 10; i++) {
		rect[i]->Render();
	}
}

void Game::Destroy()
{
	for (int i = 0; i < 10; i++) {
		SAFE_DELETE(rect[i]);
	}
	SAFE_DELETE(player);
}

void Game::IsCollision()
{
	for (int i = 0; i < 10; i++) {
		comPosition[i] = rect[i]->GetPosition();
		comScale[i] = rect[i]->GetScale();
	}
	userPosition = player->GetPosition();
	userScale = player->GetScale();

	for (int i = 0; i < 10; i++) {
		float deltaX = userPosition.x - comPosition[i].x;
		float deltaY = userPosition.y - comPosition[i].y;
		
		float length = sqrt(deltaX * deltaX + deltaY * deltaY);

		if (length > userScale + comScale[i]) collResult[i] = false;
		else collResult[i] = true;
	}

	for (int i = 0; i < 10; i++) {
		if (collResult[i]) {
			player->RaiseScale(1 + comScale[i] / 500);

			SAFE_DELETE(rect[i]);
			rect[i] = new Rect(context);

			comPosition[i] = rect[i]->GetPosition();
			comScale[i] = rect[i]->GetScale();
			collResult[i] = false;
		}
	}
}
