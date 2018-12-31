#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Context *context)
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

	randPosition.x = 1280 / 14 * (rand() % 5 + 1);
	randPosition.y = 720 / 14 * (rand() % 5 + 1);

	if (rand() % 2 == 0) randPosition.x *= -1;
	if (rand() % 2 == 0) randPosition.y *= -1;

	randScale = (rand() % 5 + 1) * 15;

	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, randScale, randScale, 1);
	D3DXMatrixTranslation(&T, randPosition.x, randPosition.y, 0);

	world = S * T;

	curPosition = randPosition;
	MoveCircle();

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
		desc.FillMode = D3D11_FILL_SOLID; //WIREFRAME. 외곽선만 출력

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
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void Rect::Update()
{
	D3DXMATRIX T;

	IsCollisionEdge();

	D3DXMatrixTranslation(&T, moveDirection.x, moveDirection.y, 0);
	curPosition += moveDirection;

	world *= T;

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

	auto dc = graphics->GetDeviceContext();

	//IA단계 세팅
	dc->IASetInputLayout(inputLayout); //정보의 구간, 용도 등을 알려줌
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //정점을 어떻게 쓸것인가 -> 삼각형의 띠로 쓰겠다(정점을 공유하는 이어진 삼각형)

	//VS단계 세팅
	dc->VSSetShader(vertexShader, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cbuffer); //cbuffer넘겨줌

	//RS단계 -> 기본 세팅 되어있음
	dc->RSSetState(rsState);

	//PS단계
	dc->PSSetShader(pixelShader, nullptr, 0);

	//Draw Call(indexbuffer를 이용해 그리기 때문에 그냥 Draw로는 불가능함)
	dc->DrawIndexed(geometry.GetIndexCount(), 0, 0); //몇 개를, 몇 번부터
}

void Rect::MoveCircle()
{
	int dir = rand() % 8 + 1;

	switch (dir)
	{
	case 1: //왼쪽 위 대각선
		moveDirection.x = -3.0f;
		moveDirection.y = 3.0f;
		break;
	case 2: //위
		moveDirection.x = 0;
		moveDirection.y = 3.0f;
		break;
	case 3: //오른쪽 위 대각선
		moveDirection.x = 3.0f;
		moveDirection.y = 3.0f;
		break;
	case 4: //오른쪽
		moveDirection.x = 3.0f;
		moveDirection.y = 0;
		break;
	case 5: //오른쪽 아래 대각선
		moveDirection.x = 3.0f;
		moveDirection.y = -3.0f;
		break;
	case 6: //아래
		moveDirection.x = 0;
		moveDirection.y = -3.0f;
		break;
	case 7: //왼쪽 아래 대각선
		moveDirection.x = -3.0f;
		moveDirection.y = -3.0f;
		break;
	case 8: //왼쪽
		moveDirection.x = -3.0f;
		moveDirection.y = 0;
		break;
	}
}

void Rect::IsCollisionEdge()
{
	if (curPosition.x <= -1280 / 2 + randScale || curPosition.x >= 1280 / 2 - randScale) moveDirection.x = -moveDirection.x;
	if (curPosition.y <= -720 / 2 + randScale || curPosition.y >= 720 / 2 - randScale) moveDirection.y = -moveDirection.y;
}
