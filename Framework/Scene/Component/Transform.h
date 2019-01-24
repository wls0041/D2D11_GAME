#pragma once

class Transform
{
public:
	Transform(class Context *context);
	virtual ~Transform();

private:
	class Context *context;

	Vector3 localScale;
	Vector3 localRotation;
	Vector3 localPosition;

	Matrix local;
	Matrix world;
};