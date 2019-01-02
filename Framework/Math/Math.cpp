#include "stdafx.h"
#include "Math.h"

const float Math::Random(const float & min, const float & max)
{
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float diff = max - min;
	float val = random * diff;

	return min + val;
}

const int Math::Random(const int & min, const int & max)
{
	return rand() % (max - min + 1) + min;
}

/*
ceil
floor
round
*/