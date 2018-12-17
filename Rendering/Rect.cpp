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

		bufferDesc.Usage = D3D11_USAGE_DEFAULT; //만든 버퍼를 어떠한 형태로 사용할 것인가, 가장 기본값으로 사용하겠다
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //만든 버퍼를 어떠한 버퍼로 묶을 것인가, vertex buffer로 묶겠다
		bufferDesc.ByteWidth = sizeof(VertexColor) * 4;//버퍼의 크기를 어느 정도로 할 것인가

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = vertices;

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&bufferDesc, &subData, &vertexBuffer); //두 데이터를 가지고 vertexBuffer를 생성
		assert(SUCCEEDED(hr));
	}

	//Create Vertex Shader
	{
		//Color.hlsl에 들어있는 VS를 vs_5버전으로 컴파일해서 vsBlob에 넣어준다.
		HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*으로 받겠다, FileA - char*로 받겠다
			"Color.hlsl", //src(source) 원본파일,  dest(destination) 복사파일
			nullptr, nullptr,
			"VS", //우리가 만든 함수 이름
			"vs_5_0",  //컴파일 수준
			0, 0, nullptr,
			&vsBlob,  //byte코드
			nullptr, nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreateVertexShader(
			vsBlob->GetBufferPointer(), //시작점부터
			vsBlob->GetBufferSize(),    //사이즈 크기까지
			nullptr,
			&vertexShader
		);
		assert(SUCCEEDED(hr));
	}

	//Create Index Data
	{
		indices = new uint[6]{ 0, 1, 2, 2, 1, 3 }; //정점은 4개여도 정점의 정보를 받을 건 6개가 필요.
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		/*
		default : cpu에서는 이 데이터를 접근 및 읽을 수 없다. 읽고 쓰기 cpu.
		dynamic : 실시간으로 갱신시키기 위해 사용. gpu가 읽기만 함. 쓰는 건 cpu가. 조금 느림.
		staging : gpu에서 쓰는 데이터를 cpu에 복사 가능. cpu는 읽기만 가능. 매우 느림.
		immutable : 읽기 gpu. cpu가 접근조차 할 수 없음. 만들 때 초기화 필요.
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
		HRESULT hr = D3DX11CompileFromFileA( //FileW - wstring*으로 받겠다, FileA - char*로 받겠다
			"Color.hlsl", //src(source) 원본파일,  dest(destination) 복사파일
			nullptr, nullptr,
			"PS", //우리가 만든 함수 이름
			"ps_5_0",  //컴파일 수준
			0, 0, nullptr,
			&psBlob,
			nullptr, nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreatePixelShader(
			psBlob->GetBufferPointer(), //시작점부터
			psBlob->GetBufferSize(),    //사이즈 크기까지
			nullptr,
			&pixelShader
		);
		assert(SUCCEEDED(hr));
	}

	//Create InputLayout
	{
		//layout을 만드는데 vertex shader가 들어가는 이유 : 셰이더의 시멘틱과 엘리먼트의 시멘틱이 일치하는지 확인하기 위함
		HRESULT hr = graphics->GetDevice()->CreateInputLayout(VertexColor::descs, VertexColor::count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		assert(SUCCEEDED(hr));
	}

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
	uint stride = sizeof(VertexColor); //정점 한 개의 크기
	uint offset = 0; //시작점

	auto dc = graphics->GetDeviceContext();

	//IA단계 세팅
	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset); //정점정보 삽입
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); //4byte uint하나를 쓰므로 r32_uint
	dc->IASetInputLayout(inputLayout); //정보의 구간, 용도 등을 알려줌
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cbuffer); //cbuffer넘겨줌

	//RS단계 -> 기본 세팅 되어있음
	dc->RSSetState(rsState);

	//PS단계
	dc->PSSetShader(pixelShader, nullptr, 0);

	//OM단계 -> back buffer를 가진 graphic클래스에서 OMSet을 해주고 있음

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(6, 0, 0); //몇 개를, 몇 번부터
}
