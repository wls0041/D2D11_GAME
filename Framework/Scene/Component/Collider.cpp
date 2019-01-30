#include "stdafx.h"
#include "Collider.h"

Collider::Collider(Context * context) : context(context), transform(nullptr), Event(nullptr), center(0.0f), size(1.0f)
{
}

Collider::~Collider()
{
}

const BoundBox Collider::GetBoundBox() const
{
	Vector3 minBox = Vector3(center - size * 0.5f);
	Vector3 maxBox = Vector3(center + size * 0.5f);

	return BoundBox(minBox, maxBox);
}

const bool Collider::AABB(Collider * collider, const int &caseNum)
{
	BoundBox boundBox = GetBoundBox();

	auto check = boundBox.IsInside(collider->GetBoundBox(), caseNum);

	return static_cast<uint>(check) > 0;
}

void Collider::Update()
{
}
