#pragma once
#include "stdafx.h"

//subsystem들의 type을 하나로 묶기 위한 준 인터페이스
class ISubsystem
{
public:
	ISubsystem(class Context *context) : context(context) {}
	virtual ~ISubsystem() {} //상속시 자식의 소멸자 호출이 되지 않는 문제 해결을 위해 가상으로 생성

	virtual void Initialize() = 0; //오버라이딩 필요

protected:
	class Context *context; //이게 없으면 완전한 인터페이스
};