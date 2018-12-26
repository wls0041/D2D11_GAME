#include "stdafx.h"
#include "Game.h"
#include "./Rendering/Rect.h"

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

	rect = new Rect(graphics);
}

void Game::Update()
{
	D3DXMatrixTranspose(&data.View, &view);
	D3DXMatrixTranspose(&data.Projection, &projection);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//��, ���. ��->�� �̻� ������ ���ϵ��� ����(update �߿� cbuffer�� �� �ǵ��)(�������� ����)->subResource�� ��� ���� �����ͼ� �־���,  ���->���� ���� ǰ. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource�� �����ض�, ������ ������(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //����, ����, ������ ũ��
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);

	rect->Update();
}

void Game::Render()
{
	auto dc = graphics->GetDeviceContext();
	dc->VSSetConstantBuffers(0, 1, &cbuffer); //cbuffer�Ѱ���

	rect->Render();
}

void Game::Destroy()
{
	SAFE_DELETE(rect);
}
