#include "stdafx.h"
#include "Transform.h"

Transform::Transform(Context * context) : context(context), localScale(1, 1, 1), localRotation(0, 0, 0), localPosition(0, 0, 0)
{
	local.SetIdentity();
	world.SetIdentity();
}

Transform::~Transform()
{
}
