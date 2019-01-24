#pragma once

class Vector3
{
public: //static -> 헤더파일만 있으면 접근가능
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
	static const float Distance(const Vector3 &lhs, const Vector3 &rhs); //루트연산 후 반환
	static const float DistanceSq(const Vector3 &lhs, const Vector3 &rhs);//제곱 상태로 반환
	static const float Dot(const Vector3 &lhs, const Vector3 &rhs);//ABcosTheta가 나옴

	static const Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);//순서 중요, 수직 벡터, 처음-다음 감싸고 엄지방향, 왼손오른손좌표계중요
	static const Vector3 Transform(const Vector3 &lhs, const class Matrix &rhs); //동차를 나누지 않고 뺌
	static const Vector3 TransformCoord(const Vector3 &lhs, const class Matrix &rhs); //위치, 동차1
	static const Vector3 TransformNormal(const Vector3 &lhs, const class Matrix &rhs);//벡터(방향), 동차0

public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(const float &x, const float &y, const float &z) : x(x), y(y), z(z) {}
	Vector3(const float &rhs) : x(rhs), y(rhs), z(rhs) {}
	Vector3(const Vector3 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {} //복사생성자
	~Vector3() {}

	operator float *() { return&x; } //암시적 변환 method
	operator const float *() const { return&x; }

	const float LengthSq() const { return sqrt(x*x + y * y + z * z); } //벡터의 크기(원점부터의 길이)
	const float Length() const { return x*x + y * y + z * z; }
	const float Volume() const { return x * y * z; }
	const float Dot(const Vector3 &rhs) { return Dot(*this, rhs); }

	const Vector3 Cross(const Vector3 &rhs) { return Cross(*this, rhs); }
	const Vector3 Absolute() { return Vector3(fabs(x), fabs(y), fabs(z)); }
	const Vector3 Normalize() const; //내건 안바꾸고 값만 반환
	void Normalize(); //크기를 1로 -> 방향값, 내걸 바꿈

	const Vector3 operator+(const Vector3 &rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	const Vector3 operator-(const Vector3 &rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	const Vector3 operator*(const Vector3 &rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
	const Vector3 operator/(const Vector3 &rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

	//scalar연산
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
	const bool operator!=(const Vector3 &rhs) const { return !(*this == rhs); } //this포인터 -> 자기자신

public: //vec.x, vec.y처럼 바로 접근하기 위함
	float x;
	float y;
	float z;
};