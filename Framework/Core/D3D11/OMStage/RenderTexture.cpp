#include "stdafx.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture(Context * context) : rtvTexture(nullptr), rtv(nullptr), srv(nullptr), format(DXGI_FORMAT_R8G8B8A8_UNORM), width(0), height(0)
{
	graphics = context->GetSubsystem<Graphics>();
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}

RenderTexture::~RenderTexture()
{
	Clear();
}

void RenderTexture::Create(const uint & width, const uint & height, const DXGI_FORMAT & format)
{   //하나의 텍스쳐로 srv와 rtv를 동시에 연동시킴. 하나만 바꿔도 나머지 하나에 영향을 줌.
	if (width == 0 || height == 0) assert(false);

	this->width = width;
	this->height = height;
	this->format = format;

	//Create Viewport
	{
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	//Create Render Target Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		//create texture2d
		HRESULT hr = graphics->GetDevice()->CreateTexture2D(&desc, nullptr, &rtvTexture);
		assert(SUCCEEDED(hr));
	}

	//Create RenderTargetView
	{
		CD3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(CD3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = graphics->GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv);
		assert(SUCCEEDED(hr));
	}

	//Create ShaderResourceView
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2D.MostDetailedMip = 0;

		HRESULT hr = graphics->GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &srv);
		assert(SUCCEEDED(hr));
	}
}

void RenderTexture::Clear()
{
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(rtvTexture);
	
	height = 0;
	width = 0;
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}

void RenderTexture::ClearTarget(const Color & color)
{
	graphics->GetDeviceContext()->ClearRenderTargetView(rtv, color); //operator 암시적 형변환 만들어놔서 바로 넣을 수 있음
}

void RenderTexture::SetTarget()
{
	graphics->GetDeviceContext()->RSSetViewports(1, &viewport);
	graphics->GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
}

void RenderTexture::Save(const string & filePath)
{
	HRESULT hr = D3DX11SaveTextureToFileA(graphics->GetDeviceContext(), rtvTexture, D3DX11_IFF_PNG, filePath.c_str());
	assert(SUCCEEDED(hr));
}
