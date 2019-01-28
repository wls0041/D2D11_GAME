#include "stdafx.h"
#include "DirectWrite.h"

DirectWrite::DirectWrite(Context * context) : ISubsystem(context), factory(nullptr), writeFactory(nullptr), writeDevice(nullptr), writeDeviceContext(nullptr), targetBitmap(nullptr)
{
}

DirectWrite::~DirectWrite()
{
}

void DirectWrite::Initialize()
{
	auto graphics = context->GetSubsystem<Graphics>();

	//Create Write Factory, __uuidof->interface ID 빼오는 놈
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	assert(SUCCEEDED(hr));

	//Create D2D Factory
	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_NONE;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	assert(SUCCEEDED(hr));

	//Get d3d Information
	IDXGIDevice *dxgiDevice = nullptr;
	hr = graphics->GetDevice()->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	//Create Write Devid
	hr = factory->CreateDevice(dxgiDevice, &writeDevice);
	assert(SUCCEEDED(hr));

	//Create Write Device Context
	hr = writeDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &writeDeviceContext); //최적화 -> 글자 찍는 속도 매우 증가
	assert(SUCCEEDED(hr));

	CreateSurface();
}

void DirectWrite::CreateSurface()
{
	auto graphics = context->GetSubsystem<Graphics>();
	IDXGISurface *dxgiSurface = nullptr;
	HRESULT hr = graphics->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiSurface));
	assert(SUCCEEDED(hr));
	
	//Create d2d Backbuffer (surface)
	D2D1_BITMAP_PROPERTIES1 bp; //속성, desc같은 놈
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96; //1bit에 들어가는 pixel 수
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW; //render target, 글자 찍는 용도이기 때문에 그리기 막음
	bp.colorContext = nullptr;

	hr = writeDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);
	assert(SUCCEEDED(hr));

	writeDeviceContext->SetTarget(targetBitmap);
	SAFE_RELEASE(dxgiSurface);
}

void DirectWrite::DeleteSurface()
{
}

void DirectWrite::BeginTextDraw()
{
}

void DirectWrite::EndTextDraw()
{
}
