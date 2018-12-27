#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Context *context)
{
	graphics = context->GetSubsystem<Graphics>();

	//Create Vertex Data, Create Index Data
	GeometryUtility::CreateQuad(geometry);

	//create vertex buffer
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	//create index buffer
	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	//Create Vertex Shader
	{
		//Color.hlsl�� ����ִ� VS�� vs_5�������� �������ؼ� vsBlob�� �־��ش�.
		HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*���� �ްڴ�, FileA - char*�� �ްڴ�
			"TexCoord.hlsl", //src(source) ��������,  dest(destination) ��������
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
			"TexCoord.hlsl", //src(source) ��������,  dest(destination) ��������
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
		HRESULT hr = graphics->GetDevice()->CreateInputLayout(VertexTexture::descs, VertexTexture::count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
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

	//Crate Texture
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(graphics->GetDevice(), L"Tree.png", nullptr, nullptr, &diffuseMap, nullptr);
		assert(SUCCEEDED(hr));

		hr = D3DX11CreateShaderResourceViewFromFile(graphics->GetDevice(), L"Forest.png", nullptr, nullptr, &diffuseMap2, nullptr);
		assert(SUCCEEDED(hr));
	}

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = true; //alpha�� ���� ����
		desc.IndependentBlendEnable = false; //���� rendertarget�� ����� ��� ������ state�� ������ ���� ���ڴ°�. false�� 0�� ������ ��� ����
		desc.RenderTarget[0].BlendEnable = false; //blend�� ���ΰ�


		//�ϴ� ���. ������ ȥ��
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //ȥ�հ��Ŀ� ������ �� ���� ��� ���� ���ΰ�(�� ���� ���϶�)

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//


		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //�׸��� ���� �־��� ���� ��� 

		HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &blendState);
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
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Rect::Update()
{
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 1280, 720, 1);
	D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(45)));
	D3DXMatrixTranslation(&T, 0, 0, 0);

	world = S;// *R * T;

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
	//dc->RSSetState(rsState);

	//PS�ܰ�
	dc->PSSetShader(pixelShader, nullptr, 0);
	dc->PSSetShaderResources(0, 1, &diffuseMap); //�Ϻ� �����ϰ� PS�ܰ迡�� ����
	dc->PSSetShaderResources(1, 1, &diffuseMap2); 

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //�� ����, �� ������
}
