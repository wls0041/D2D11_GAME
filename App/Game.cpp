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
		//�־��� y, z���� ���� CROSS(����)������ �Ͽ� x���� �˾Ƴ�, ���� ���� ����� �־��� ������ ��������(A X B��� �޼����� a���� b�� ��������� �� ���� ����)
		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, ��  
	}

	//Create View Matrix
	{
		D3DXMatrixOrthoLH(&projection, 1280, 720, 0, 1);  //������ ������ ������ ����, ī�޶� �� - zn(near plane), ������ �� - zf(far plane)
		//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1);
	}
	//������� ������ ��ȯ��Ű�� ���� VS. ���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)

	rect = new Rect(context);
}

void Game::Update()
{
	auto data = static_cast<CameraData*>(cameraBuffer->Map()); //������ ��Ʈ�� ������. ������ �����Ƿ� cameraBuffer�� ĳ����

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
