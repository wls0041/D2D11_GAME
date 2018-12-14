#pragma once

//기본으로 창 띄우는 것은 api, 그 위에 dx를 얹음

//실제로 랜더링 하기 위한 클래스
class Graphics
{
public:
	Graphics();
	virtual ~Graphics();

	ID3D11Device *GetDevice() const { return device; }
	ID3D11DeviceContext *GetDeviceContext() const { return deviceContext; }
	IDXGISwapChain *GetSwapChain() const { return swapChain; }

	void Initialize();

	void BeginScene();
	void EndScene();

private:
	void CreateBackBuffer(const uint &width, const uint &height); //창 사이즈가 뱌뀌면 다시 사이즈 조정을 하기 위해 생성자에서 초기화가 아닌 함수로 선언.

private:
	//I : com interface -> dx가 미리 만들어 놓은 함수 가짐. 생성은 create로, 삭제는 release, desc를 가짐(하나의 구조체)
	
	/*
	I없이 d3dx붙는 놈들 D3DX(Direct용이다)
	D3DXVECTOR2 - 2차원 벡터
	D3DXVECTO3  - 3차원 벡터
	D3DXCOLOR   - rgba가지는 색

	동차. 좌표와 벡터의 구분이 안되기 때문에 만든 개념
	현 차원에 하나의 차원을 추가함.(1차원->2차원, 2차원->3차원)
	이 추가된 차원을 '동차'라 함. (2차원->(x,y,z)) z = 0:방향, 1:공간
	*/

	ID3D11Device *device;              //d3d핵심. 요소를 생성
	ID3D11DeviceContext *deviceContext;//d3d핵심. 요소를 세팅하거나 그림. pipeline에 요소를 꽂음
	IDXGISwapChain *swapChain;         //d3d핵심. (간단히)더블버퍼링 해줌

	ID3D11RenderTargetView *rtv; //뭔가를 그리는 대상(도화지), view가 붙었다면->Texture
	D3D11_VIEWPORT viewport;     //랜더링할 구간, 구역을 지정
	D3DXCOLOR clearColor;        //directX10
};