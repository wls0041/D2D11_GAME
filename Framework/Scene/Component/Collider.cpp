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

const bool Collider::AABB(Collider * collider)
{
	BoundBox boundBox = GetBoundBox();

	auto check = boundBox.IsInside(collider->GetBoundBox());
	return static_cast<uint>(check) > 0;
}

const CircleCheck Collider::AABB_Circle(Collider * collider, const CheckCase & caseNum)
{
	BoundBox boundBox = GetBoundBox();
	BoundBox colliderBox = collider->GetBoundBox();
	CircleCheck check;

	switch (caseNum)
	{
	case CheckCase::Rect_Circle:
		check = boundBox.IsInside_Circle(colliderBox);
		break;
	case CheckCase::Circle_Rect:
		check = colliderBox.IsInside_Circle(boundBox);
		break;
	case CheckCase::Back_Circle:
		check = boundBox.IsOutside_Circle(colliderBox);
		break;
	}

	return check;
}

void Collider::Update()
{
}
