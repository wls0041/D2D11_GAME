#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Graphics * graphics) : graphics(graphics)
{
	//Create Vertex Data
	{
		/*
		13  012
		02  213
		*/
		//0
		vertices = new VertexColor[4];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[0].color = D3DXCOLOR(1, 0, 0, 1);

		//1
		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[1].color = D3DXCOLOR(0, 1, 0, 1);

		//2
		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[2].color = D3DXCOLOR(0, 0, 1, 1);

		//3
		vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		vertices[3].color = D3DXCOLOR(0, 0, 1, 1);
	}

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT; //���� ���۸� ��� ���·� ����� ���ΰ�, ���� �⺻������ ����ϰڴ�
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� ���۸� ��� ���۷� ���� ���ΰ�, vertex buffer�� ���ڴ�
		bufferDesc.ByteWidth = sizeof(VertexColor) * 4;//������ ũ�⸦ ��� ������ �� ���ΰ�

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

	//Create Index Data
	{
		indices = new uint[6]{ 0, 1, 2, 2, 1, 3 }; //������ 4������ ������ ������ ���� �� 6���� �ʿ�.
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		/*
		default : cpu������ �� �����͸� ���� �� ���� �� ����. �а� ���� cpu.
		dynamic : �ǽð����� ���Ž�Ű�� ���� ���. gpu�� �б⸸ ��. ���� �� cpu��. ���� ����.
		staging : gpu���� ���� �����͸� cpu�� ���� ����. cpu�� �б⸸ ����. �ſ� ����.
		immutable : �б� gpu. cpu�� �������� �� �� ����. ���� �� �ʱ�ȭ �ʿ�.
		*/
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint) * 6;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = indices;

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &subData, &indexBuffer);
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
		//layout�� ����µ� vertex shader�� ���� ���� : ���̴��� �ø�ƽ�� ������Ʈ�� �ø�ƽ�� ��ġ�ϴ��� Ȯ���ϱ� ����
		HRESULT hr = graphics->GetDevice()->CreateInputLayout(VertexColor::descs, VertexColor::count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		assert(SUCCEEDED(hr));
	}

	//���� ������� �ʱ�ȭ
	D3DXMatrixIdentity(&world);

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
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //�ո��� ��� �Ǻ��� ���ΰ�. false = �ð������ �ո�
		desc.CullMode = D3D11_CULL_NONE; //�ø�. ������ �ʴ� �κ�(BACK, FRONT, NONE)�� ������ �����ս��� ����
		desc.FillMode = D3D11_FILL_WIREFRAME; //WIREFRAME. �ܰ����� ���

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}

}

Rect::~Rect()
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

void Rect::Update()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 100, 100, 1);
	D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(45)));
	D3DXMatrixTranslation(&T, 0, 0, 0);

	world = S * R * T;

	//��켱�� ���켱���� �ٲ���(��ġ)
	D3DXMatrixTranspose(&data.World, &world);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//��, ���. ��->�� �̻� ������ ���ϵ��� ����(update �߿� cbuffer�� �� �ǵ��)(�������� ����)->subResource�� ��� ���� �����ͼ� �־���,  ���->���� ���� ǰ. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource�� �����ض�, ������ ������(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //����, ����, ������ ũ��
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);
}

void Rect::Render()
{
	uint stride = sizeof(VertexColor); //���� �� ���� ũ��
	uint offset = 0; //������

	auto dc = graphics->GetDeviceContext();

	//IA�ܰ� ����
	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset); //�������� ����
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); //4byte uint�ϳ��� ���Ƿ� r32_uint
	dc->IASetInputLayout(inputLayout); //������ ����, �뵵 ���� �˷���
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cbuffer); //cbuffer�Ѱ���

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	dc->RSSetState(rsState);

	//PS�ܰ�
	dc->PSSetShader(pixelShader, nullptr, 0);

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(6, 0, 0); //�� ����, �� ������
}
