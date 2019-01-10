#pragma once

class Camera
{
public:
	Camera(class Context *context);
	virtual ~Camera();

	const D3DXMATRIX &GetViewMatrix() const { return view; }
	const D3DXMATRIX &GetProjectionMatrix() const { return projection; }
	const D3DXVECTOR3 &GetPosition() const { return position; }

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	class Context *context;
	class Timer *timer;
	class Input *input;

	D3DXVECTOR3 position;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 forward;

	D3DXMATRIX view;
	D3DXMATRIX projection;

	float zoom;
};