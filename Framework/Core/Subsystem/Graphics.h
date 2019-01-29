//#pragma once
//
////�⺻���� â ���� ���� api, �� ���� dx�� ����
//
////������ ������ �ϱ� ���� Ŭ����
//class Graphics
//{
//public:
//	Graphics();
//	virtual ~Graphics();
//
//	ID3D11Device *GetDevice() const { return device; }
//	ID3D11DeviceContext *GetDeviceContext() const { return deviceContext; }
//	IDXGISwapChain *GetSwapChain() const { return swapChain; }
//
//	void Initialize();
//
//	void BeginScene();
//	void EndScene();
//
//private:
//	void CreateBackBuffer(const uint &width, const uint &height); //â ����� ���� �ٽ� ������ ������ �ϱ� ���� �����ڿ��� �ʱ�ȭ�� �ƴ� �Լ��� ����.
//
//private:
//	//I : com interface -> dx�� �̸� ����� ���� �Լ� ����. ������ create��, ������ release, desc�� ����(�ϳ��� ����ü)
//	
//	/*
//	I���� d3dx�ٴ� ��� D3DX(Direct���̴�)
//	Vector2 - 2���� ����
//	D3DXVECTO3  - 3���� ����
//	Color   - rgba������ ��
//
//	����. ��ǥ�� ������ ������ �ȵǱ� ������ ���� ����
//	�� ������ �ϳ��� ������ �߰���.(1����->2����, 2����->3����)
//	�� �߰��� ������ '����'�� ��. (2����->(x,y,z)) z = 0:����, 1:����
//	*/
//
//	ID3D11Device *device;              //d3d�ٽ�. ��Ҹ� ����
//	ID3D11DeviceContext *deviceContext;//d3d�ٽ�. ��Ҹ� �����ϰų� �׸�. pipeline�� ��Ҹ� ����
//	IDXGISwapChain *swapChain;         //d3d�ٽ�. (������)������۸� ����
//
//	ID3D11RenderTargetView *rtv; //������ �׸��� ���(��ȭ��), view�� �پ��ٸ�->Texture
//	D3D11_VIEWPORT viewport;     //�������� ����, ������ ����
//	Color clearColor;        //directX10
//};

#pragma once
#include "../Subsystem/ISubsystem.h"

class Graphics : public ISubsystem
{
public:
	Graphics(class Context* context);
	virtual ~Graphics();

	ID3D11Device* GetDevice() const { return device; }
	ID3D11DeviceContext* GetDeviceContext() const { return deviceContext; }
	IDXGISwapChain* GetSwapChain() const { return swapChain; } //�׷����� ���� ��� ����

	void Initialize() override;

	void ResizeClient(const uint& width, const uint& height);
	void BeginScene();
	void EndScene();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	ID3D11Debug* debugDevice;
	uint gpuMemorySize;
	wstring gpuDescription;
	uint numerator;
	uint denominator;

	Color clearColor;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RenderTargetView* renderTargetView;
	D3D11_VIEWPORT viewport;
};
