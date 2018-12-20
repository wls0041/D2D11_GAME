#pragma once
#include "stdafx.h"

//subsystem���� type�� �ϳ��� ���� ���� �� �������̽�
class ISubsystem
{
public:
	ISubsystem(class Context *context) : context(context) {}
	virtual ~ISubsystem() {} //��ӽ� �ڽ��� �Ҹ��� ȣ���� ���� �ʴ� ���� �ذ��� ���� �������� ����

	virtual void Initialize() = 0; //�������̵� �ʿ�

protected:
	class Context *context; //�̰� ������ ������ �������̽�
};