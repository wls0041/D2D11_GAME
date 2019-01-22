#pragma once

class Transform
{
public:
	Transform(class Context *context);
	virtual ~Transform();

private:
	class Context *context;

	D3DXVECTOR3 localScale;
	D3DXVECTOR3 localRotation;
	D3DXVECTOR3 localPosition;

	D3DXMATRIX local;
	D3DXMATRIX world;
};