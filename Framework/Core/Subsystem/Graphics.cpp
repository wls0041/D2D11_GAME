#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics() : device(nullptr), deviceContext(nullptr), swapChain(nullptr), rtv(nullptr), clearColor(0xff555566)
{
}

Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
}

void Graphics::Initialize()
{
	DXGI_MODE_DESC bufferDesc; //swapchain에 들어감. 더블 버퍼링을 위해 설정
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	 
	bufferDesc.Width = 1280;
	bufferDesc.Height = 720;
	bufferDesc.RefreshRate.Numerator = 60; //갱신
	bufferDesc.RefreshRate.Denominator = 1;//시간당
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsinged normalize intiger, 데이터의 자료형에 따라 bit와 format을 바꿔씀, 1byte(8bit)로 씀. 4byte라면 32bit 2^32개의 색 사용 가능
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 

	DXGI_SWAP_CHAIN_DESC swapChainDesc; //swapchain을 만들기 위한 desc
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc; 
	swapChainDesc.SampleDesc.Count = 1; //기본값 - 사실상 하지 않겠다
	swapChainDesc.SampleDesc.Quality = 0;//기본값
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //출력하는 용도로 쓸 것이다
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = Settings::Get().GetWindowHandle(); //window의 handle이 들어갈 자리
	swapChainDesc.Windowed = TRUE; //창모드 사용할 것인가
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //더블 버퍼링 시 바뀌고 난 정보는 파기하겠다(저장x)
	//Sampling
	//ssaa(super sampling anti-ailiasing. 작은 이미지를 크게 만든 후 다시 축소하여 선명히 함). 화질은 높아지나 비용이 많이 듬 
	//msaa(multi sampling ~~). 이미지의 외곽부분만 찾아서 조정. 크게 느렸다가 줄이는 방식 
	//계단현상(ailiasing)

	D3D_FEATURE_LEVEL featureLevels[] = { //기능레벨. d3d9 ~ 11까지 쓰겠다
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain( //device, context, swapchain을 한 번에 생성
		nullptr, D3D_DRIVER_TYPE_HARDWARE, //컴퓨터의 하드웨어가 지원이 안되도 가상으로 만들 수 있게 한다. -> 우리는 쓰지 않음
		nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,//신호. 어떤 신호를 줄 것인가
		featureLevels, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain, &device,
		nullptr, &deviceContext
	);
	assert(SUCCEEDED(hr));
	
	CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
}

void Graphics::BeginScene() //clear
{
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); //세팅
	deviceContext->ClearRenderTargetView(rtv, clearColor);//세팅된 곳에 clearColor로 칠함. context그리거나 삭제 등
}

void Graphics::EndScene() //clipping
{
	HRESULT hr = swapChain->Present(1, 0); //clipping과 같은 역할
	assert(SUCCEEDED(hr));
}

void Graphics::CreateBackBuffer(const uint & width, const uint & height)
{
	HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	//DX의 가장 기본 데이터형 -> vector, 필요한 형태가 너무 많아 기본형을 주지 않고 사용자가 직접 설정하게끔 함
	//backbuffer원본의 텍스쳐(DESC의 역할도 함) -> 추후 파이프라인에 집어넣을 때 변형 필요(ID3D11RenderTargetView로 변형)
	ID3D11Texture2D *backBuffer = nullptr; //그러나 텍스쳐는 한정적이기 때문에 텍스쳐에 한해서 기본형 제공
	
	//2번째. IID : interface ID, uuidof. textrue2d안에 있는 id빼옴, 3번째. 텍스쳐의 표면, void**형으로 받는데 명시적 변환이 안되어 강제 변환 해줌
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);  //resource에 상속되어 있음. backbuffer넣으면 알아서 업캐스팅됨
	assert(SUCCEEDED(hr));

	//viewport -> 보여줄 영역(랜더링할 영역)
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport); //viewport 개수, 변수
}
