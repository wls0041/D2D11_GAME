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

	vertexShader = new VertexShader(context);
	vertexShader->Create("TexCoord.hlsl");

	pixelShader = new PixelShader(context);
	pixelShader->Create("TexCoord.hlsl");

	//공간 단위행렬 초기화
	D3DXMatrixIdentity(&world);

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

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false; //앞면을 어떻게 판별할 것인가. false = 시계방향이 앞면
		desc.CullMode = D3D11_CULL_NONE; //컬링. 원하지 않는 부분(BACK, FRONT, NONE)은 생략해 퍼포먼스를 높임
		desc.FillMode = D3D11_FILL_WIREFRAME; //WIREFRAME. 외곽선만 출력

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
		desc.AlphaToCoverageEnable = true; //alpha의 적용 범위
		desc.IndependentBlendEnable = false; //여러 rendertarget을 사용할 경우 각각의 state의 설정을 따로 쓰겠는가. false면 0의 설정을 모두 적용
		desc.RenderTarget[0].BlendEnable = false; //blend할 것인가


		//일단 사용. 색들의 혼합
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //혼합공식에 나오는 두 색을 어떻게 섞을 것인가(두 개를 더하라)

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//


		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //그리는 곳에 주어진 색만 써라 

		HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &blendState);
		assert(SUCCEEDED(hr));
	}
}

Rect::~Rect()
{
	SAFE_RELEASE(cbuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_DELETE(pixelShader);
	SAFE_DELETE(vertexShader); 
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

	//행우선을 열우선으로 바꿔줌(전치)
	D3DXMatrixTranspose(&data.World, &world);

	D3D11_MAPPED_SUBRESOURCE subResource;
	//맵, 언맵. 맵->더 이상 들어오지 못하도록 막음(update 중에 cbuffer를 못 건들게)(나가지도 못함)->subResource가 대신 값을 가져와서 넣어줌,  언맵->막은 것을 품. 
	graphics->GetDeviceContext()->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //subResource에 복사해라, 썼으면 버려라(discard)
	memcpy(subResource.pData, &data, sizeof(Data)); //복사, 원본, 복사할 크기
	graphics->GetDeviceContext()->Unmap(cbuffer, 0);
}

void Rect::Render()
{
	vertexBuffer->BindPipeline();
	indexBuffer->BindPipeline();
	vertexShader->BindPipeline();
	pixelShader->BindPipeline();

	auto dc = graphics->GetDeviceContext();

	//IA단계 세팅
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅
	dc->VSSetConstantBuffers(1, 1, &cbuffer); //cbuffer넘겨줌

	//RS단계 -> 기본 세팅 되어있음
	//dc->RSSetState(rsState);

	//PS단계
	dc->PSSetShaderResources(0, 1, &diffuseMap); //일부 제외하고 PS단계에서 삽입
	dc->PSSetShaderResources(1, 1, &diffuseMap2); 

	//OM단계 -> back buffer를 가진 graphic클래스에서 OMSet을 해주고 있음
	dc->OMSetBlendState(blendState, nullptr, 0xff);

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //몇 개를, 몇 번부터
}
