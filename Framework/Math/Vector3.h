#pragma once

class Vector3
{
public: //static -> ������ϸ� ������ ���ٰ���
	static const Vector3 Zero;
	static const Vector3 One; 
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Forward;
	static const Vector3 Back;
	static const Vector3 Infinity;
	static const Vector3 NegInfinity;

public:
	static const float Distance(const Vector3 &lhs, const Vector3 &rhs); //��Ʈ���� �� ��ȯ
	static const float DistanceSq(const Vector3 &lhs, const Vector3 &rhs);//���� ���·� ��ȯ
	static const float Dot(const Vector3 &lhs, const Vector3 &rhs);//ABcosTheta�� ����

	static const Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);//���� �߿�, ���� ����, ó��-���� ���ΰ� ��������, �޼տ�������ǥ���߿�
	static const Vector3 Transform(const Vector3 &lhs, const class Matrix &rhs); //������ ������ �ʰ� ��
	static const Vector3 TransformCoord(const Vector3 &lhs, const class Matrix &rhs); //��ġ, ����1
	static const Vector3 TransformNormal(const Vector3 &lhs, const class Matrix &rhs);//����(����), ����0

public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(const float &x, const float &y, const float &z) : x(x), y(y), z(z) {}
	Vector3(const float &rhs) : x(rhs), y(rhs), z(rhs) {}
	Vector3(const Vector3 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {} //���������
	~Vector3() {}

	operator float *() { return&x; } //�Ͻ��� ��ȯ method
	operator const float *() const { return&x; }

	const float LengthSq() const { return sqrt(x*x + y * y + z * z); } //������ ũ��(���������� ����)
	const float Length() const { return x*x + y * y + z * z; }
	const float Volume() const { return x * y * z; }
	const float Dot(const Vector3 &rhs) { return Dot(*this, rhs); }

	const Vector3 Cross(const Vector3 &rhs) { return Cross(*this, rhs); }
	const Vector3 Absolute() { return Vector3(fabs(x), fabs(y), fabs(z)); }
	const Vector3 Normalize() const; //���� �ȹٲٰ� ���� ��ȯ
	void Normalize(); //ũ�⸦ 1�� -> ���Ⱚ, ���� �ٲ�

	const Vector3 operator+(const Vector3 &rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	const Vector3 operator-(const Vector3 &rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	const Vector3 operator*(const Vector3 &rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
	const Vector3 operator/(const Vector3 &rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

	//scalar����
	const Vector3 operator+(const float &rhs) const { return Vector3(x + rhs, y + rhs, z + rhs); }
	const Vector3 operator-(const float &rhs) const { return Vector3(x - rhs, y - rhs, z - rhs); }
	const Vector3 operator*(const float &rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	const Vector3 operator/(const float &rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

	void operator+=(const Vector3 &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; }
	void operator-=(const Vector3 &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
	void operator*=(const Vector3 &rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; }
	void operator/=(const Vector3 &rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; }

	void operator+=(const float &rhs) { x += rhs; y += rhs; z += rhs; }
	void operator-=(const float &rhs) { x -= rhs; y -= rhs; z -= rhs; }
	void operator*=(const float &rhs) { x *= rhs; y *= rhs; z *= rhs; }
	void operator/=(const float &rhs) { x /= rhs; y /= rhs; z /= rhs; }

	const bool operator==(const Vector3 &rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	const bool operator!=(const Vector3 &rhs) const { return !(*this == rhs); } //this������ -> �ڱ��ڽ�

public: //vec.x, vec.yó�� �ٷ� �����ϱ� ����
	float x;
	float y;
	float z;
};