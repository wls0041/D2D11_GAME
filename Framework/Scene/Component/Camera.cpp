#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Context * context) : context(context), position(0, 0, 0), right(1, 0, 0), up(0, 1, 0), forward(0, 0, 1), zoom(1.0f)
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
	if ((input->KeyPress(VK_SHIFT))) {
		if (input->KeyPress('W')) position += up * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('S')) position -= up * 200.0f * timer->GetDeltaTimeSec();
		if (input->KeyPress('A')) position -= right * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('D')) position += right * 200.0f * timer->GetDeltaTimeSec();
		
		float delta = input->GetMouseMoveValue().z;
		if (Math::Abs(delta) >= WHEEL_DELTA) {
			float deltaZoom = 1.0f - Math::Abs(delta) / WHEEL_DELTA / 10.0f;

			if (delta < 0) zoom *= deltaZoom;
			else zoom /= deltaZoom;

			zoom = Math::clamp(zoom, 0.1f, 100.0f);
		}
	}

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
	D3DXMatrixOrthoLH(&projection, viewport.Width / zoom, viewport.Height / zoom, 0.0f, 1.0f);
	//D3DXMatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, �� 
	//������� ������ ��ȯ��Ű�� ���� VS.���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)
}
