#include "stdafx.h"
#include "Vector3.h"

const Vector3 Vector3::Zero(0.0f);
const Vector3 Vector3::One(1.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Back(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Infinity(numeric_limits<float>::infinity());
const Vector3 Vector3::NegInfinity(-numeric_limits<float>::infinity());

const float Vector3::Distance(const Vector3 & lhs, const Vector3 & rhs)
{
	Vector3 delta = lhs - rhs;
	return sqrt(pow(delta.x, 2) + (delta.y, 2) + (delta.z, 2));
}

const float Vector3::DistanceSq(const Vector3 & lhs, const Vector3 & rhs)
{
	Vector3 delta = lhs - rhs;
	return pow(delta.x, 2) + (delta.y, 2) + (delta.z, 2);
}

const float Vector3::Dot(const Vector3 & lhs, const Vector3 & rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

const Vector3 Vector3::Cross(const Vector3 & lhs, const Vector3 & rhs)
{
	float num1 = lhs.y * rhs.z - lhs.z * rhs.y;
	float num2 = lhs.z * rhs.x - lhs.x * rhs.z;
	float num3 = lhs.x * rhs.y - lhs.y * rhs.x;

	return Vector3(num1, num2, num3);
}

const Vector3 Vector3::Normalize() const
{
	float factor = Length();
	factor = 1.0f / factor;

	return Vector3(x, y, z) * factor;
}

void Vector3::Normalize()
{
	float factor = Length();
	factor = 1.0f / factor;

	x *= factor;
	y *= factor;
	z *= factor;
}
