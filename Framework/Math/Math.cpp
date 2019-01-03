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
	//������ FLOAT���� ������ rand�ִ밪(32767)�� ����. 
	//'0~�ִ밪 / �ִ밪'�̹Ƿ� 0~1 ������ ������ ���� �� ����. %���� /�� ȿ����
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 
	float diff = max - min; //�������� �ּҰ��� �ִ밪�� ������ ���
	float val = random * diff; //������ ������ �������� ����. ���ϴ� ������ ���� ����
	                           //ex)0~1 * 0.5(0.8 - 0.3) = 0~0.5

	return min + val; //������ ������ �ּҰ��� ���� ���� ����.
}

const int Math::Random(const int & min, const int & max)
{
	return rand() % (max - min + 1) + min;
}
