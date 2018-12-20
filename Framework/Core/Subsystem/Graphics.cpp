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
	DXGI_MODE_DESC bufferDesc; //swapchain�� ��. ���� ���۸��� ���� ����
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	 
	bufferDesc.Width = 1280;
	bufferDesc.Height = 720;
	bufferDesc.RefreshRate.Numerator = 60; //����
	bufferDesc.RefreshRate.Denominator = 1;//�ð���
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsinged normalize intiger, �������� �ڷ����� ���� bit�� format�� �ٲ㾸, 1byte(8bit)�� ��. 4byte��� 32bit 2^32���� �� ��� ����
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 

	DXGI_SWAP_CHAIN_DESC swapChainDesc; //swapchain�� ����� ���� desc
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc; 
	swapChainDesc.SampleDesc.Count = 1; //�⺻�� - ��ǻ� ���� �ʰڴ�
	swapChainDesc.SampleDesc.Quality = 0;//�⺻��
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //����ϴ� �뵵�� �� ���̴�
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = Settings::Get().GetWindowHandle(); //window�� handle�� �� �ڸ�
	swapChainDesc.Windowed = TRUE; //â��� ����� ���ΰ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //���� ���۸� �� �ٲ�� �� ������ �ı��ϰڴ�(����x)
	//Sampling
	//ssaa(super sampling anti-ailiasing. ���� �̹����� ũ�� ���� �� �ٽ� ����Ͽ� ������ ��). ȭ���� �������� ����� ���� �� 
	//msaa(multi sampling ~~). �̹����� �ܰ��κи� ã�Ƽ� ����. ũ�� ���ȴٰ� ���̴� ��� 
	//�������(ailiasing)

	D3D_FEATURE_LEVEL featureLevels[] = { //��ɷ���. d3d9 ~ 11���� ���ڴ�
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain( //device, context, swapchain�� �� ���� ����
		nullptr, D3D_DRIVER_TYPE_HARDWARE, //��ǻ���� �ϵ��� ������ �ȵǵ� �������� ���� �� �ְ� �Ѵ�. -> �츮�� ���� ����
		nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,//��ȣ. � ��ȣ�� �� ���ΰ�
		featureLevels, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain, &device,
		nullptr, &deviceContext
	);
	assert(SUCCEEDED(hr));
	
	CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
}

void Graphics::BeginScene() //clear
{
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); //����
	deviceContext->ClearRenderTargetView(rtv, clearColor);//���õ� ���� clearColor�� ĥ��. context�׸��ų� ���� ��
}

void Graphics::EndScene() //clipping
{
	HRESULT hr = swapChain->Present(1, 0); //clipping�� ���� ����
	assert(SUCCEEDED(hr));
}

void Graphics::CreateBackBuffer(const uint & width, const uint & height)
{
	HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	//DX�� ���� �⺻ �������� -> vector, �ʿ��� ���°� �ʹ� ���� �⺻���� ���� �ʰ� ����ڰ� ���� �����ϰԲ� ��
	//backbuffer������ �ؽ���(DESC�� ���ҵ� ��) -> ���� ���������ο� ������� �� ���� �ʿ�(ID3D11RenderTargetView�� ����)
	ID3D11Texture2D *backBuffer = nullptr; //�׷��� �ؽ��Ĵ� �������̱� ������ �ؽ��Ŀ� ���ؼ� �⺻�� ����
	
	//2��°. IID : interface ID, uuidof. textrue2d�ȿ� �ִ� id����, 3��°. �ؽ����� ǥ��, void**������ �޴µ� ����� ��ȯ�� �ȵǾ� ���� ��ȯ ����
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);  //resource�� ��ӵǾ� ����. backbuffer������ �˾Ƽ� ��ĳ���õ�
	assert(SUCCEEDED(hr));

	//viewport -> ������ ����(�������� ����)
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport); //viewport ����, ����
}
