#include "stdafx.h"
#include "Anim.h"
#include "../Scene/Component/Animator.h"

Anim::Anim(Context *context) : scale(1, 1, 1), position(0, 0, 0), rotate(0, 0, 0), size(0, 0), offset(0, 0)
{
	graphics = context->GetSubsystem<Graphics>();
	auto resourceMgr = context->GetSubsystem<ResourceManager>();

	//Create Vertex Data, Create Index Data
	GeometryUtility::CreateQuad(geometry);

	//create vertex buffer
	vertexBuffer = new VertexBuffer(context);
	vertexBuffer->Create<VertexTexture>(geometry.GetVertices());

	//create index buffer
	indexBuffer = new IndexBuffer(context);
	indexBuffer->Create(geometry.GetIndices());

	vertexShader = new VertexShader(context);
	vertexShader->Create("../_Assets/Shader/SpriteTexture.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("../_Assets/Shader/SpriteTexture.hlsl");

	inputLayout = new InputLayout(context);
	inputLayout->Create(vertexShader->GetBlob());

	worldBuffer = new ConstantBuffer(context);
	worldBuffer->Create<WorldData>();

	//Create Texture
	texture = resourceMgr->Load<Texture>("bird.png");

	//���� ������� �ʱ�ȭ
	D3DXMatrixIdentity(&world);

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

	////////////////////////////////////////////////////////////////////////////////////////////
	spriteBuffer = new ConstantBuffer(context);
	spriteBuffer->Create<AnimationData>();
}

Anim::~Anim()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Anim::Update()
{
	scale.x = 1280 / size.y;
	scale.y = 720 / size.y;

	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotate.y, rotate.x, rotate.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * R * T;

	auto data = static_cast<WorldData*>(worldBuffer->Map());
	//��켱�� ���켱���� �ٲ���(��ġ)
	D3DXMatrixTranspose(&data->World, &world);
	worldBuffer->Unmap();

	auto animData = static_cast<AnimationData*>(spriteBuffer->Map());
	animData->TextureSize = texture->GetSize();
	animData->SpriteOffset = offset;
	animData->Spritesize = size;
	spriteBuffer->Unmap();
}

void Anim::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	inputLayout->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();
	worldBuffer->BindPipeline(ShaderType::VS, 1); //0 : camera
	spriteBuffer->BindPipeline(ShaderType::VS, 2);
	texture->BindPipeline(ShaderType::PS, 0);

	auto dc = graphics->GetDeviceContext();

	//IA�ܰ� ����
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //������ ��� �����ΰ� -> �ﰢ���� ��� ���ڴ�(������ �����ϴ� �̾��� �ﰢ��)

	//VS�ܰ� ����

	//RS�ܰ� -> �⺻ ���� �Ǿ�����
	//dc->RSSetState(rsState);

	//PS�ܰ�

	//OM�ܰ� -> back buffer�� ���� graphicŬ�������� OMSet�� ���ְ� ����
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer�� �̿��� �׸��� ������ �׳� Draw�δ� �Ұ�����)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //�� ����, �� ������
}