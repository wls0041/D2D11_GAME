#pragma once

class Vector2 final
{//총 12byte (virtual(4), float(4), float(4)) -> virtual을 사용하지 않음 -> final로 상속불가하도록 묶음 --> 8byte고정됨
public:
	static const Vector2 Zero; //전역 쓸 수 있도록 static, 값을 묶기위해 const
	static const Vector2 One;

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(const float &x, const float &y) : x(x), y(y) {}
	Vector2(const float &rhs) : x(rhs), y(rhs) {}
	Vector2(const Vector2 &rhs) : x(rhs.x), y(rhs.y) {} //복사생성자
	~Vector2() {}

	operator float *() { return&x; } //암시적 변환 method
	operator const float *() const { return&x; }

	const Vector2 operator+(const Vector2 &rhs) { return Vector2(x + rhs.x, y + rhs.y); }
	const Vector2 operator-(const Vector2 &rhs) { return Vector2(x - rhs.x, y - rhs.y);	}
	const Vector2 operator*(const Vector2 &rhs) { return Vector2(x * rhs.x, y * rhs.y); }
	const Vector2 operator/(const Vector2 &rhs) { return Vector2(x / rhs.x, y / rhs.y); }

	//scalar연산
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

public: //vec.x, vec.y처럼 바로 접근하기 위함
	float x;
	float y;
};