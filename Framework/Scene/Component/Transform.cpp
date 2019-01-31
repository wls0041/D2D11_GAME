#include "stdafx.h"
#include "Transform.h"

Transform::Transform(Context * context) : context(context), localScale(1.0f), localRotation(0.0f), localPosition(0.0f), parent(nullptr)
{
	local.SetIdentity();
	world.SetIdentity();
}

Transform::~Transform()
{
}

const Matrix Transform::GetWorldRotationMatrix()
{
	Vector3 scale = GetScale();

	bool bCheck = false;
	bCheck |= scale.x == 0.0f;
	bCheck |= scale.y == 0.0f;
	bCheck |= scale.z == 0.0f;

	if (bCheck) return Matrix();

	float factorX = 1.0f / scale.x;
	float factorY = 1.0f / scale.y;
	float factorZ = 1.0f / scale.z;

	return Matrix
	(
		world._11 * factorX, world._12 * factorX, world._13 * factorX, 0.0f,
		world._21 * factorY, world._22 * factorY, world._23 * factorY, 0.0f,
		world._31 * factorZ, world._32 * factorZ, world._33 * factorZ, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void Transform::SetLocalScale(const Vector3 & localScale)
{
	if (this->localScale == localScale) return;
	this->localScale = localScale;
	UpdateTransform();
}

void Transform::SetLocalRotation(const Vector3 & localRotation)
{
	if (this->localRotation == localRotation) return;
	this->localRotation = localRotation;
	UpdateTransform();
}

void Transform::SetLocalPosition(const Vector3 & localPosition)
{
	if (this->localPosition == localPosition) return;
	this->localPosition = localPosition;
	UpdateTransform();
}

void Transform::SetScale(const Vector3 & scale)
{
	if (GetScale() == scale) return;
	if (HasParent()) {
		Vector3 parentScale = parent->GetScale();
		Vector3 s = scale / parentScale;
		SetLocalScale(s);
	}
	else SetLocalScale(scale);
}

void Transform::SetRotation(const Vector3 & rotation)
{
	if (GetRotation() == rotation) return;
	if (HasParent()) {
		Matrix inv = Matrix::Inverse(parent->GetWorldRotationMatrix()); //역행렬 곱하면 해당 행렬이 단위행렬로 변함
		SetLocalRotation(Vector3::TransformNormal(rotation, inv)); //회전은 방향이므로 normal
	}
	else SetLocalRotation(rotation);
}

void Transform::SetPosition(const Vector3 & position)
{
	if (GetPosition() == position) return;
	if (HasParent()) {
		Matrix inv = Matrix::Inverse(parent->GetWorldMatrix()); //역행렬 곱하면 해당 행렬이 단위행렬로 변함
		SetLocalPosition(Vector3::TransformCoord(position, inv));
	}
	else SetLocalPosition(position);
}

void Transform::SetParent(Transform * newParent)
{
	this->parent = newParent;
	newParent->AddChild(this);
}

void Transform::AddChild(Transform * child)
{
	childs.emplace_back(child);
}

void Transform::UpdateTransform()
{
	Matrix S = Matrix::Scaling(localScale);
	Matrix R = Matrix::RotationYawPitchRoll(localRotation);
	Matrix T = Matrix::Translation(localPosition);

	local = S * R * T; //자식만의 world,  world->parent행렬을 곱한 것

	if (HasParent()) world = local * parent->GetWorldMatrix();
	else world = local;

	//참조가 안붙어도 됨. 동적이므로. 정적이면 붙여야함. 우리는 childs의 복사를 만들지 않고 그 주소 그대로 쓰기 위해 &을 붙임
	for (auto &child : childs) child->UpdateTransform(); //자식이 없으면 vector가 0이므로 확인할 필요 없음
}
