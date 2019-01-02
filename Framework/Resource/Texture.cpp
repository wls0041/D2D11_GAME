#include "stdafx.h"
#include "Texture.h"

Texture::Texture(Context * context) : srv(nullptr), format(DXGI_FORMAT_R8G8B8A8_UNORM), filePath(""), width(0), height(0)
{
	graphics = context->GetSubsystem<Graphics>();
}

Texture::~Texture()
{
	Clear();
}

void Texture::CreateTextureFromFile(const string & filePath)
{
	this->filePath = filePath;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(graphics->GetDevice(), filePath.c_str(), nullptr, nullptr, &srv, nullptr);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D *srcTexture = nullptr;
	srv->GetResource(reinterpret_cast<ID3D11Resource **>(&srcTexture)); //srv에서  texture2d을 꺼내옴
	
	D3D11_TEXTURE2D_DESC desc;
	srcTexture->GetDesc(&desc); //꺼내온 src에서 desc꺼내옴

	width = static_cast<float>(desc.Width);
	height = static_cast<float>(desc.Height);
	format = desc.Format;
}

void Texture::CreateTexture2D(const float & width, const float & height, const DXGI_FORMAT & format) //어떻게 돌아가는지 확인용. 사용x
{	
	//이 내용을 위의 D3DX11CreateShaderResourceViewFromFileA가 대신 해줌
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = static_cast<uint>(width);
	texDesc.Height = static_cast<uint>(height);
	texDesc.MipLevels = 1; //텍스쳐 축소 생성. 안씀
	texDesc.ArraySize = 1; //텍스쳐를 sprite처럼 배열로 한 번에 밀어넣음
	texDesc.Format = format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0; //안씀
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //render_target도 같이 쓰고 싶다면 | D3D11_BIND_SHADER_RESOURCE를 추가
	texDesc.CPUAccessFlags = 0; //DYNAMIC이 아니므로
	texDesc.MiscFlags = 0; //기타(etc)

	ID3D11Texture2D *texture = nullptr;//아무것도 없는 기본값에 칠할 수 있음
	HRESULT hr = graphics->GetDevice()->CreateTexture2D(&texDesc, nullptr, &texture);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D; //텍스쳐를 무엇으로 만들었는지 알려줌 (2D로 만들었다)
	srvDesc.Texture2D.MostDetailedMip = 0; //mipMap관련 알 필요 없음
	srvDesc.Texture2D.MipLevels = 1;
	
	hr = graphics->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv);
	assert(SUCCEEDED(hr));
}

void Texture::Clear()
{
	SAFE_RELEASE(srv);
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	width = 0.0f;
	height = 0.0f;
}

void Texture::BindPipeline(const ShaderType & type, const uint & slot)
{
	auto dc = graphics->GetDeviceContext();

	switch (type)
	{
	case ShaderType::VS: dc->VSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::PS: dc->PSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::GS: dc->GSSetShaderResources(slot, 1, &srv); break;
	case ShaderType::CS: dc->CSSetShaderResources(slot, 1, &srv); break;
	}
}
