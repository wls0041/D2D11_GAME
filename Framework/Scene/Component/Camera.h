#pragma once

class Camera
{
public:
	Camera(class Context *context);
	virtual ~Camera();

	const Matrix &GetViewMatrix() const { return view; }
	const Matrix &GetProjectionMatrix() const { return projection; }
	const Vector3 &GetPosition() const { return position; }

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	class Context *context;
	class Timer *timer;
	class Input *input;

	Vector3 position;

	Matrix view;
	Matrix projection;

	float zoom;
};