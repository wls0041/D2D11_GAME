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
