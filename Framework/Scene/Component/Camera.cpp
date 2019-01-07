#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Context * context) : context(context), position(0, 0, 0), right(1, 0, 0), up(0, 1, 0), forward(0, 0, 1)
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();

	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&projection);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	//주어진 y, z축을 토대로 CROSS(외적)연산을 하여 x축을 알아냄, 외적 연산 결과는 주어진 벡터의 수직벡터(A X B라면 왼손으로 a에서 b로 감아쥐었을 때 엄지 방향)
	D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up); //투영될 공간의 사이즈 지정, 카메라 앞 - zn(near plane), 투영된 곳 - zf(far plane)
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = Settings::Get().GetViewPort();
	D3DXMatrixOrthoLH(&projection, viewport.Width, viewport.Height, 0.0f, 1.0f);
	//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1); //반환 받을 변수, 카메라 위치, 바라보는 방향, 위 
	//만들어진 공간을 변환시키는 것은 VS.따라서 VS로 가서 만듬(color.hlsld의 cbuffer와 vs), 보낼 때 정보를 보내줘야함(id3d11buffer)
}
