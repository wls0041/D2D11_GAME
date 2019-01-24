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
	//주어진 y, z축을 토대로 CROSS(외적)연산을 하여 x축을 알아냄, 외적 연산 결과는 주어진 벡터의 수직벡터(A X B라면 왼손으로 a에서 b로 감아쥐었을 때 엄지 방향)
	view = Matrix::LookAtLH(position, (position + Vector3::Forward), Vector3::Up); //투영될 공간의 사이즈 지정, 카메라 앞 - zn(near plane), 투영된 곳 - zf(far plane)
}

void Camera::UpdateProjectionMatrix()
{
	auto viewport = Settings::Get().GetViewPort();
	projection = Matrix::OrthoLH(viewport.Width / zoom, viewport.Height / zoom, 0.0f, 1.0f);
	//MatrixOrthoOffCenterLH(&projection, 0, 1280, 720, 0, 0,  1); //반환 받을 변수, 카메라 위치, 바라보는 방향, 위 
	//만들어진 공간을 변환시키는 것은 VS.따라서 VS로 가서 만듬(color.hlsld의 cbuffer와 vs), 보낼 때 정보를 보내줘야함(id3d11buffer)
}
