#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Context * context) : context(context), position(0, 0, 0), zoom(1.0f)
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();

	view.SetIdentity();
	projection.SetIdentity();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if ((input->KeyPress(VK_SHIFT))) {
		if (input->KeyPress('W')) position += Vector3::Up * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('S')) position -= Vector3::Up * 200.0f * timer->GetDeltaTimeSec();
		if (input->KeyPress('A')) position -= Vector3::Right * 200.0f * timer->GetDeltaTimeSec();
		else if (input->KeyPress('D')) position += Vector3::Right * 200.0f * timer->GetDeltaTimeSec();
		
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
	view = Matrix::LookAtLH(position, (position + Vector3::Forward), Vector3::Up); //������ ������ ������ ����, ī�޶� �� - zn(near plane), ������ �� - zf(far plane)
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = Settings::Get().GetViewPort();
	projection = Matrix::OrthoLH(viewport.Width / zoom, viewport.Height / zoom, 0.0f, 1.0f);
	//MatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1); //��ȯ ���� ����, ī�޶� ��ġ, �ٶ󺸴� ����, �� 
	//������� ������ ��ȯ��Ű�� ���� VS.���� VS�� ���� ����(color.hlsld�� cbuffer�� vs), ���� �� ������ ���������(id3d11buffer)
}
