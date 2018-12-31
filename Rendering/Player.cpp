#include "stdafx.h"
#include "Player.h"

Player::Player(Context *context)
{
	graphics = context->GetSubsystem<Graphics>();

	//Create Vertex Data, Create Index Data
	GeometryUtility::CreateCircle(geometry);

	//create vertex buffer
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexColor>(geometry.GetVertices());

	//create index buffer
	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

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
		//layout�� ����µ� vertex shader�� ���� ���� : ���̴��� �ø�ƽ�� ������Ʈ�� �ø�ƽ�� ��ġ�ϴ��� Ȯ���ϱ� ����
		HRESULT hr = graphics->GetDevice()->CreateInputLayout(VertexColor::descs, VertexColor::count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		assert(SUCCEEDED(hr));
	}
	//���� ������� �ʱ�ȭ
	D3DXMatrixIdentity(&world);

	curScale = 30.0f;
	curPosition.x = 0;
	curPosition.y = 0;

	D3DXMATRIX S, T;
	D3DXMatrixScaling(&S, curScale, curScale, 1);
	D3DXMatrixTranslation(&T, curPosition.x, curPosition.y, 0);

	world = S * T;

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
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. �ܰ����� ���

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);
		assert(SUCCEEDED(hr));
	}
}

Player::~Player()
{
	SAFE_RELEASE(cbuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(psBlob);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(vsBlob);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Player::Update()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		if (GetAsyncKeyState('W') & 0x8000) if(!IsCollisionEdge(LT)) MoveCircle(D3DXVECTOR2(0.0f, 3.0f));
		if (GetAsyncKeyState('S') & 0x8000) if (!IsCollisionEdge(LB)) MoveCircle(D3DXVECTOR2(0.0f, -3.0f));
		
		if (!IsCollisionEdge(LL)) MoveCircle(D3DXVECTOR2(-3.0f, 0.0f));
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		if (GetAsyncKeyState('W') & 0x8000) if (!IsCollisionEdge(RT)) MoveCircle(D3DXVECTOR2(0.0f, 3.0f));
		if (GetAsyncKeyState('S') & 0x8000) if (!IsCollisionEdge(RB)) MoveCircle(D3DXVECTOR2(0.0f, -3.0f));
		
		if (!IsCollisionEdge(RR)) MoveCircle(D3DXVECTOR2(3.0f, 0.0f));
	}
	else if (GetAsyncKeyState('W') & 0x8000)
	{
		if (GetAsyncKeyState('A') & 0x8000) if (!IsCollisionEdge(LT)) MoveCircle(D3DXVECTOR2(-3.0f, 0.0f));
		if (GetAsyncKeyState('D') & 0x8000) if (!IsCollisionEdge(RT)) MoveCircle(D3DXVECTOR2(3.0f, 0.0f));
		
		if (!IsCollisionEdge(TT)) MoveCircle(D3DXVECTOR2(0.0f, 3.0f)); 
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		if (GetAsyncKeyState('A') & 0x8000) if (!IsCollisionEdge(LB)) MoveCircle(D3DXVECTOR2(-3.0f, 0.0f));
		if (GetAsyncKeyState('D') & 0x8000) if (!IsCollisionEdge(RB)) MoveCircle(D3DXVECTOR2(3.0f, 0.0f));
		
		if (!IsCollisionEdge(BB)) MoveCircle(D3DXVECTOR2(0.0f, -3.0f)); 
	}

	//��켱�� ���켱���� �ٲ���(��ġ)
	D3DXMatrixTranspose(&data.World, &world);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//��, ���. ��->�� �̻� ������ ���ϵ��� ����(update �߿� cbuffer�� �� �ǵ��)(�������� ����)->subResource�� ��� ���� �����ͼ� �־���,  ���->���� ���� ǰ. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource�� �����ض�, ������ ������(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //����, ����, ������ ũ��
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);
}

void Player::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();

	auto dc = graphics->GetDeviceContext();

	//IA�ܰ� ����
	dc->IASetInputLayout(inputLayout); //������ ����, �뵵 ���� �˷���
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cbuffer); //cbuffer�Ѱ���

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	dc->RSSetState(rsState);

	//PS�ܰ�
	dc->PSSetShader(pixelShader, nullptr, 0);

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //�� ����, �� ������
}

bool Player::IsCollisionEdge(int dir)
{
	switch (dir)
	{
	case LT:
		if(curPosition.x < (-1280 / 2) + curScale || curPosition.y > (720 / 2) - curScale) return true;
	case TT:
		if (curPosition.y > (720 / 2) - curScale) return true;
	case RT:
		if (curPosition.x > (1280 / 2) - curScale || curPosition.y > (720 / 2) - curScale) return true;
	case RR:
		if (curPosition.x > (1280 / 2) - curScale) return true;
	case RB:
		if (curPosition.x > (1280 / 2) - curScale || curPosition.y < (-720 / 2) + curScale) return true;
	case BB:
		if (curPosition.y < (-720 / 2) + curScale) return true;
	case LB:
		if (curPosition.x < (-1280 / 2) + curScale || curPosition.y < (-720 / 2) + curScale) return true;
	case LL:
		if (curPosition.x < (-1280 / 2) + curScale) return true;
	}
	return false;
}

void Player::RaiseScale(float scale)
{
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, scale, scale, 1);
	curScale *= scale;

	world *= S;
}

void Player::MoveCircle(D3DXVECTOR2 dir)
{
	D3DXMATRIX T;

	D3DXMatrixTranslation(&T, dir.x, dir.y, 0);
	curPosition += dir;

	world *= T;
}
