#include "stdafx.h"
#include "Game.h"
#include "./Framework/Rendering/Rect.h"

void Game::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();

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

	rect = new Rect(context);
}

void Game::Update()
{
	auto data = static_cast<CameraData*>(cameraBuffer->Map()); //꼬봉의 노트를 내보냄. 형식이 없으므로 cameraBuffer로 캐스팅

	D3DXMatrixTranspose(&data->View, &view);
	D3DXMatrixTranspose(&data->Projection, &projection);

	cameraBuffer->Unmap();

	rect->Update();
}

void Game::Render()
{
	cameraBuffer->BindPipeline(ShaderType::VS, 0);
	rect->Render();
}

void Game::Destroy()
{
	SAFE_DELETE(cameraBuffer);
	SAFE_DELETE(rect);
}
