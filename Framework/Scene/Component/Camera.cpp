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
	//�־��� y, z���� ���� CROSS(����)������ �Ͽ� x���� �˾Ƴ�, ���� ���� ����� �־��� ������ ��������(A X B��� �޼����� a���� b�� ��������� �� ���� ����)
	D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up); //������ ������ ������ ����, ī�޶� �� - zn(near plane), ������ �� - zf(far plane)
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = Settings::Get().GetViewPort();
	D3DXMatrixOrthoLH(&projection, viewport.Width, viewport.Height, 0.0f, 1.0f);
	//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, �� 
	//������� ������ ��ȯ��Ű�� ���� VS.���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)
}
