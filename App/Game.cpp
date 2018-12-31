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
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //카메라가 조금이라도 움직이면 실시간으로 반영되어야 함
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.ByteWidth = sizeof(Data);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //usage가 dynamic일 때 추가. cpu를 쓰기용으로 접근하라(계산은 gpu가 다 해주므로)

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &cbuffer);
		assert(SUCCEEDED(hr));
	}

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
	for (int i = 0; i < 10; i++){
		rect[i] = new Rect(context);
	}

}

void Game::Update()
{
	if (player->GetScale() > 450) MessageBoxA(NULL, "플레이어의 크기가 한계에 도달!!", "~~~~ THE END ~~~~", MB_OK);
	IsCollision();

	D3DXMatrixTranspose(&data.View, &view);
	D3DXMatrixTranspose(&data.Projection, &projection);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//맵, 언맵. 맵->더 이상 들어오지 못하도록 막음(update 중에 cbuffer를 못 건들게)(나가지도 못함)->subResource가 대신 값을 가져와서 넣어줌,  언맵->막은 것을 품. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource에 복사해라, 썼으면 버려라(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //복사, 원본, 복사할 크기
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);

	player->Update();
	for (int i = 0; i < 10; i++) {
		rect[i]->Update();
	}
}

void Game::Render()
{
	auto dc = graphics->GetDeviceContext();
	dc->VSSetConstantBuffers(0, 1, &cbuffer); //cbuffer넘겨줌

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
