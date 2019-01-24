#pragma once

class Vector2 final
{//�� 12byte (virtual(4), float(4), float(4)) -> virtual�� ������� ���� -> final�� ��ӺҰ��ϵ��� ���� --> 8byte������
public:
	static const Vector2 Zero; //���� �� �� �ֵ��� static, ���� �������� const
	static const Vector2 One;

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(const float &x, const float &y) : x(x), y(y) {}
	Vector2(const float &rhs) : x(rhs), y(rhs) {}
	Vector2(const Vector2 &rhs) : x(rhs.x), y(rhs.y) {} //���������
	~Vector2() {}

	operator float *() { return&x; } //�Ͻ��� ��ȯ method
	operator const float *() const { return&x; }

	const Vector2 operator+(const Vector2 &rhs) { return Vector2(x + rhs.x, y + rhs.y); }
	const Vector2 operator-(const Vector2 &rhs) { return Vector2(x - rhs.x, y - rhs.y);	}
	const Vector2 operator*(const Vector2 &rhs) { return Vector2(x * rhs.x, y * rhs.y); }
	const Vector2 operator/(const Vector2 &rhs) { return Vector2(x / rhs.x, y / rhs.y); }

	//scalar����
	const Vector2 operator+(const float &rhs) { return Vector2(x + rhs, y + rhs); }
	const Vector2 operator-(const float &rhs) { return Vector2(x - rhs, y - rhs); }
	const Vector2 operator*(const float &rhs) { return Vector2(x * rhs, y * rhs); }
	const Vector2 operator/(const float &rhs) { return Vector2(x / rhs, y / rhs); }

	void operator+=(const Vector2 &rhs) { x += rhs.x; y += rhs.y; }
	void operator-=(const Vector2 &rhs) { x -= rhs.x; y -= rhs.y; }
	void operator*=(const Vector2 &rhs) { x *= rhs.x; y *= rhs.y; }
	void operator/=(const Vector2 &rhs) { x /= rhs.x; y /= rhs.y; }

	void operator+=(const float &rhs) { x += rhs; y += rhs; }
	void operator-=(const float &rhs) { x -= rhs; y -= rhs; }
	void operator*=(const float &rhs) { x *= rhs; y *= rhs; }
	void operator/=(const float &rhs) { x /= rhs; y /= rhs; }

	const bool operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
	const bool operator!=(const Vector2 &rhs) const { return x != rhs.x || y != rhs.y; }

public: //vec.x, vec.yó�� �ٷ� �����ϱ� ����
	float x;
	float y;
};