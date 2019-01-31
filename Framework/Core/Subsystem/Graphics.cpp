//#include "stdafx.h"
//#include "Graphics.h"
//
//Graphics::Graphics() : device(nullptr), deviceContext(nullptr), swapChain(nullptr), rtv(nullptr), clearColor(0xff555566)
//{
//}
//
//Graphics::~Graphics()
//{
//	SAFE_RELEASE(rtv);
//	SAFE_RELEASE(deviceContext);
//	SAFE_RELEASE(device);
//	SAFE_RELEASE(swapChain);
//}
//
//void Graphics::Initialize()
//{
//	DXGI_MODE_DESC bufferDesc; //swapchain에 들어감. 더블 버퍼링을 위해 설정
//	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
//	 
//	bufferDesc.Width = 1280;
//	bufferDesc.Height = 720;
//	bufferDesc.RefreshRate.Numerator = 60; //갱신
//	bufferDesc.RefreshRate.Denominator = 1;//시간당
//	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsinged normalize intiger, 데이터의 자료형에 따라 bit와 format을 바꿔씀, 1byte(8bit)로 씀. 4byte라면 32bit 2^32개의 색 사용 가능
//	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 
//
//	DXGI_SWAP_CHAIN_DESC swapChainDesc; //swapchain을 만들기 위한 desc
//	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
//
//	swapChainDesc.BufferDesc = bufferDesc; 
//	swapChainDesc.SampleDesc.Count = 1; //기본값 - 사실상 하지 않겠다
//	swapChainDesc.SampleDesc.Quality = 0;//기본값
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //출력하는 용도로 쓸 것이다
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.OutputWindow = Settings::Get().GetWindowHandle(); //window의 handle이 들어갈 자리
//	swapChainDesc.Windowed = TRUE; //창모드 사용할 것인가
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //더블 버퍼링 시 바뀌고 난 정보는 파기하겠다(저장x)
//	//Sampling
//	//ssaa(super sampling anti-ailiasing. 작은 이미지를 크게 만든 후 다시 축소하여 선명히 함). 화질은 높아지나 비용이 많이 듬 
//	//msaa(multi sampling ~~). 이미지의 외곽부분만 찾아서 조정. 크게 느렸다가 줄이는 방식 
//	//계단현상(ailiasing)
//
//	D3D_FEATURE_LEVEL featureLevels[] = { //기능레벨. d3d9 ~ 11까지 쓰겠다
//		D3D_FEATURE_LEVEL_11_1,
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_10_0,
//		D3D_FEATURE_LEVEL_9_3,
//		D3D_FEATURE_LEVEL_9_2,
//		D3D_FEATURE_LEVEL_9_1
//	};
//
//	HRESULT hr = D3D11CreateDeviceAndSwapChain( //device, context, swapchain을 한 번에 생성
//		nullptr, D3D_DRIVER_TYPE_HARDWARE, //컴퓨터의 하드웨어가 지원이 안되도 가상으로 만들 수 있게 한다. -> 우리는 쓰지 않음
//		nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,//신호. 어떤 신호를 줄 것인가
//		featureLevels, 1, D3D11_SDK_VERSION,
//		&swapChainDesc, &swapChain, &device,
//		nullptr, &deviceContext
//	);
//	assert(SUCCEEDED(hr));
//	
//	CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
//}
//
//void Graphics::BeginScene() //clear
//{
//	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); //세팅
//	deviceContext->ClearRenderTargetView(rtv, clearColor);//세팅된 곳에 clearColor로 칠함. context그리거나 삭제 등
//}
//
//void Graphics::EndScene() //clipping
//{
//	HRESULT hr = swapChain->Present(1, 0); //clipping과 같은 역할
//	assert(SUCCEEDED(hr));
//}
//
//void Graphics::CreateBackBuffer(const uint & width, const uint & height)
//{
//	HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
//	assert(SUCCEEDED(hr));
//
//	//DX의 가장 기본 데이터형 -> vector, 필요한 형태가 너무 많아 기본형을 주지 않고 사용자가 직접 설정하게끔 함
//	//backbuffer원본의 텍스쳐(DESC의 역할도 함) -> 추후 파이프라인에 집어넣을 때 변형 필요(ID3D11RenderTargetView로 변형)
//	ID3D11Texture2D *backBuffer = nullptr; //그러나 텍스쳐는 한정적이기 때문에 텍스쳐에 한해서 기본형 제공
//	
//	//2번째. IID : interface ID, uuidof. textrue2d안에 있는 id빼옴, 3번째. 텍스쳐의 표면, void**형으로 받는데 명시적 변환이 안되어 강제 변환 해줌
//	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
//	assert(SUCCEEDED(hr));
//
//	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);  //resource에 상속되어 있음. backbuffer넣으면 알아서 업캐스팅됨
//	assert(SUCCEEDED(hr));
//
//	//viewport -> 보여줄 영역(랜더링할 영역)
//	viewport.TopLeftX = 0;
//	viewport.TopLeftY = 0;
//	viewport.Width = static_cast<float>(width);
//	viewport.Height = static_cast<float>(height);
//	viewport.MinDepth = 0.0f;
//	viewport.MaxDepth = 1.0f;
//
//	deviceContext->RSSetViewports(1, &viewport); //viewport 개수, 변수
//}
#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics(Context * context)
	: ISubsystem(context)
	, deviceContext(nullptr)
	, swapChain(nullptr)
	, debugDevice(nullptr)
	, gpuMemorySize(0)
	, gpuDescription(L"")
	, numerator(0)
	, denominator(0)
	, clearColor(0xFF555566)
	, depthStencilView(nullptr)
	, renderTargetView(nullptr)
{
}

Graphics::~Graphics()
{
	if (swapChain != nullptr)
		swapChain->SetFullscreenState(false, nullptr);

	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
}

void Graphics::Initialize()
{
	IDXGIFactory* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	uint modeCount;
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&modeCount,
		nullptr
	);
	assert(SUCCEEDED(hr));

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modeCount];
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED
		, &modeCount
		, displayModeList
	);
	assert(SUCCEEDED(hr));

	for (uint i = 0; i < modeCount; i++)
	{
		bool isCheck = true;
		isCheck &= displayModeList[i].Width == Settings::Get().GetWidth();
		isCheck &= displayModeList[i].Height == Settings::Get().GetHeight();

		if (isCheck == true)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));

	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	gpuMemorySize = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterDesc.Description;

	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (Settings::Get().GetIsVsync() == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = Settings::Get().GetWindowHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !Settings::Get().GetIsFullScreen();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	uint creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	   D3D_FEATURE_LEVEL_10_1,
	   D3D_FEATURE_LEVEL_10_0,
	   D3D_FEATURE_LEVEL_9_3,
	   D3D_FEATURE_LEVEL_9_2,
	   D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);
	assert(SUCCEEDED(hr));

	ResizeClient
	(
		static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight())
	);
}

void Graphics::ResizeClient(const uint & width, const uint & height)
{
	assert(device);
	assert(deviceContext);
	assert(swapChain);

	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetView);

	HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	//RenderTargetView 만들기
	ID3D11Texture2D* backBuffer = nullptr;
	hr = swapChain->GetBuffer
	(
		0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(backBuffer);

	//DepthStencilView 만들기
	ID3D11Texture2D* depthStencilBuffer = nullptr;

	D3D11_TEXTURE2D_DESC dsbDesc;
	ZeroMemory(&dsbDesc, sizeof(D3D11_TEXTURE2D_DESC));

	dsbDesc.Width = width;
	dsbDesc.Height = height;
	dsbDesc.MipLevels = 1;
	dsbDesc.ArraySize = 1;
	dsbDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsbDesc.SampleDesc.Count = 1;
	dsbDesc.SampleDesc.Quality = 0;
	dsbDesc.Usage = D3D11_USAGE_DEFAULT;
	dsbDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsbDesc.CPUAccessFlags = 0;
	dsbDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&dsbDesc, nullptr, &depthStencilBuffer);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &depthStencilView);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(Settings::Get().GetWidth());
	viewport.Height = static_cast<float>(Settings::Get().GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Settings::Get().SetViewPort(viewport);

	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void Graphics::BeginScene()
{
	assert(deviceContext != nullptr);
	assert(swapChain != nullptr);

	// 화면에 보여질 버퍼 세팅
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// 화면에 보여질 버퍼 지우기
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

	// 화면에 보여질 깊이&스텐실 버퍼 지우기
	deviceContext->ClearDepthStencilView
	(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
	);
}

void Graphics::EndScene()
{
	// 그린놈 보여주기
	HRESULT hr = swapChain->Present(Settings::Get().GetIsVsync() == true ? 1 : 0, 0);
	assert(SUCCEEDED(hr));
}