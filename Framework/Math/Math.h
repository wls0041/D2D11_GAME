#pragma once

enum class Intersection : uint { Outside, Inside, Intersect };

class Math
{
public:
	static const float PI;
	static const float PI_2;
	static const float PI_DIV_2;
	static const float PI_DIV_4;
	static const float TO_RAD;
	static const float TO_DEG;

public:
	static const float ToRadian(const float &degree) { return degree * TO_RAD; }
	static const float ToDegree(const float &degree) { return degree * TO_DEG; }
	static const float Random(const float &min, const float &max);
	static const int Random(const int &min, const int &max);

	template <typename T>
	static const T clamp(const T &value, const T &min, const T &max) { return value < min ? min : (value > max ? max : value); }

	template <typename T>
	static const T Abs(const T &value) { return value >= 0 ? value : -value; }
	
	template <typename T>
	static const T Max(const T &lhs, const T &rhs) { return lhs > rhs ? lhs : rhs; }

	template <typename T>
	static const T Min(const T &lhs, const T &rhs) { return lhs < rhs ? lhs : rhs; }

	template <typename T> //부호함수
	static const int Sign(const T &value) { return (T(0) < value) - (value < T(0)); }
	// 양수 : 1 - 0 = 1, 음수 : 0 - 1 = -1, 0 : 0 - 0 = 0.  주어진 값의 부호를 반환

	template <typename T>
	static const bool Equals(const T &lhs, const T &rhs, const T &e = numeric_limits<T>::epsilon()) { 
		return lhs + e >= rhs && lhs - e <= rhs;
	}//lhs에 극히 작은 값 e를 더하거나 빼는 것으로 rhs와의 우위가 바뀐다면 두 값을 같은 값으로 간주한다.
};