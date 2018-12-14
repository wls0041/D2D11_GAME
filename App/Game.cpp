#include "stdafx.h"
#include "Game.h"

void Game::Initialize()
{
	//Create Vertex Data
	{
		/*
		13  012
		02  213
		*/
		//0
		vertices = new VertexColor[6];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[0].color = D3DXCOLOR(1, 0, 0, 1);

		//1
		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[1].color = D3DXCOLOR(0, 1, 0, 1);

		//2
		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[2].color = D3DXCOLOR(0, 0, 1, 1);

		//2
		vertices[3].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[3].color = D3DXCOLOR(1, 0, 0, 1);

		//1
		vertices[4].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[4].color = D3DXCOLOR(0, 1, 0, 1);

		//3
		vertices[5].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		vertices[5].color = D3DXCOLOR(0, 0, 1, 1);
	}
	
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT; //���� ���۸� ��� ���·� ����� ���ΰ�, ���� �⺻������ ����ϰڴ�
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� ���۸� ��� ���۷� ���� ���ΰ�, vertex buffer�� ���ڴ�
		bufferDesc.ByteWidth = sizeof(VertexColor) * 6;//������ ũ�⸦ ��� ������ �� ���ΰ�

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = vertices;

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, &subData, &vertexBuffer); //�� �����͸� ������ vertexBuffer�� ����
		assert(SUCCEEDED(hr));
	}

	//Create Vertex Shader
	{
		//Color.hlsl�� ����ִ� VS�� vs_5�������� �������ؼ� vsBlob�� �־��ش�.
		HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*���� �ްڴ�, FileA - char*�� �ްڴ�
			"Color.hlsl", //src(source) ��������,  dest(destination) ��������
			nullptr, nullptr,
			"VS", //�츮�� ���� �Լ� �̸�
			"vs_5_0",  //������ ����
			0, 0, nullptr,
			&vsBlob,  //byte�ڵ�
			nullptr, nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreateVertexShader(
			vsBlob->GetBufferPointer(), //����������
			vsBlob->GetBufferSize(),    //������ ũ�����
			nullptr,
			&vertexShader
		);
		assert(SUCCEEDED(hr));
	}

	//Create Pixel Shader
	{
		HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*���� �ްڴ�, FileA - char*�� �ްڴ�
			"Color.hlsl", //src(source) ��������,  dest(destination) ��������
			nullptr, nullptr,
			"PS", //�츮�� ���� �Լ� �̸�
			"ps_5_0",  //������ ����
			0, 0, nullptr,
			&psBlob,
			nullptr, nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreatePixelShader(
			psBlob->GetBufferPointer(), //����������
			psBlob->GetBufferSize(),    //������ ũ�����
			nullptr,
			&pixelShader
		);
		assert(SUCCEEDED(hr));
	}

	//Create InputLayout
	{
		//���� ��������(�̾����Ƿ�), ���� ��� �ʿ�(��� �������� �������� ���̹Ƿ�)
		D3D11_INPUT_ELEMENT_DESC  layoutDesc[] = {  //vertexinput, pixelinput�� �� ���� ���Ҹ� ���� �־� �� ���� Desc�� �ʿ���
			//alignedByteOffset ���������� �� ����Ʈ ������ �������̰� �÷����� �˷���, �̸� ���� ���� ������, �������� ���� �� �� ����
			//instancing. �Ȱ��� ���� ���� �� ����� ��� ���� ������ ����°� �ƴ� �ϳ��� ���� ���� �� ���
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, //format�� ������ DXGI + FLOAT��(32bit)
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0} //12 = ������	
		};

		//layout�� ����µ� vertex shader�� ���� ���� : ���̴��� �ø�ƽ�� ������Ʈ�� �ø�ƽ�� ��ġ�ϴ��� Ȯ���ϱ� ����
		HRESULT hr = graphics->GetDevice()->CreateInputLayout(layoutDesc, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		assert(SUCCEEDED(hr));
	}

	//���� ������� �ʱ�ȭ
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);

	//Create projection Matrix
	{
		//�־��� y, z���� ���� CROSS(����)������ �Ͽ� x���� �˾Ƴ�, ���� ���� ����� �־��� ������ ��������(A X B��� �޼����� a���� b�� ��������� �� ���� ����)
		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, ��  
	}

	//Create View Matrix
	{
	//	D3DXMatrixOrthoLH(&projection, 1280, 720, 0, 1);  //������ ������ ������ ����, ī�޶� �� - zn(near plane), ������ �� - zf(far plane)
		D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 0, 720, 0, 1);
	}
	//������� ������ ��ȯ��Ű�� ���� VS. ���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)

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

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		MAKE_ZERO_TOKEN(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //�ո��� ��� �Ǻ��� ���ΰ�. false = �ð������ �ո�
		desc.CullMode = D3D11_CULL_BACK; //�ø�. ������ �ʴ� �κ�(BACK, FRONT, NONE)�� ������ �����ս��� ����
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. �ܰ����� ���

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}
}

void Game::Update()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 100, 100, 1);
	D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(45)));
	D3DXMatrixTranslation(&T, 100, 100, 0);

	world = S * R * T;

	//��켱�� ���켱���� �ٲ���(��ġ)
	D3DXMatrixTranspose(&data.World, &world);
	D3DXMatrixTranspose(&data.View, &view);
	D3DXMatrixTranspose(&data.Projection, &projection);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//��, ���. ��->�� �̻� ������ ���ϵ��� ����(update �߿� cbuffer�� �� �ǵ��)(�������� ����)->subResource�� ��� ���� �����ͼ� �־���,  ���->���� ���� ǰ. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource�� �����ض�, ������ ������(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //����, ����, ������ ũ��
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);

}

void Game::Render()
{
	uint stride = sizeof(VertexColor); //���� �� ���� ũ��
	uint offset = 0; //������

	auto dc = graphics->GetDeviceContext();
	
	//IA�ܰ� ����
	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset); //�������� ����
	dc->IASetInputLayout(inputLayout); //������ ����, �뵵 ���� �˷���
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, &cbuffer); //cbuffer�Ѱ���

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	dc->RSSetState(rsState);

	//PS�ܰ�
	dc->PSSetShader(pixelShader, nullptr, 0);

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����

	//Draw Call
	dc->Draw(6, 0); //�� ����, �� ������
}

void Game::Destroy()
{
	SAFE_RELEASE(cbuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(psBlob);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(vsBlob);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(vertices);
}
