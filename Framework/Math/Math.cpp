#include "stdafx.h"
#include "Math.h"


const float Math::PI       = 3.14159265359f;
const float Math::PI_2     = 6.28318530718f;
const float Math::PI_DIV_2 = 1.57079632679f;
const float Math::PI_DIV_4 = 0.78539816339f;
const float Math::TO_RAD   = PI / 180.f;
const float Math::TO_DEG   = 180.f / PI;

const float Math::Random(const float & min, const float & max)
{
	//랜덤한 FLOAT형의 정수를 rand최대값(32767)로 나눔. 
	//'0~최대값 / 최대값'이므로 0~1 사이의 난수를 얻을 수 있음. %보다 /이 효율적
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 
	float diff = max - min; //랜덤값의 최소값과 최대값의 범위를 계산
	float val = random * diff; //구해진 범위를 랜덤값에 곱함. 원하는 범위의 난수 생성
	                           //ex)0~1 * 0.5(0.8 - 0.3) = 0~0.5

	return min + val; //구해진 난수에 최소값을 더해 값을 맞춤.
}

const int Math::Random(const int & min, const int & max)
{
	return rand() % (max - min + 1) + min;
}
