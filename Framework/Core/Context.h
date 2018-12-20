#pragma once
#include "stdafx.h"
#include "./Subsystem/ISubsystem.h"

//subsystem���� vector�� ����
class Context final
{
public:
	Context() {}
	~Context() { for (uint i = subsystems.size() - 1; i >= 0; i--) SAFE_DELETE(subsystems[i]); }

	void RegisterSubsystem(class ISubsystem *subsystem) { subsystems.push_back(subsystem); }

	//typename�� T(����)�� �ϰ� �� Ÿ���� �Լ��� ����
	template <typename T> //����� subsystems�� ��Ҹ� ���� ������ �� ���ϴ� ���� ã�� ���� template���
	T *GetSubsystem();

private:
	vector<class ISubsystem*> subsystems;
};

template<typename T>
inline T * Context::GetSubsystem()
{
	for (auto subsystem : subsystems)
	{
		if (typeid(T) == typeid(*subsystem))
			return static_cast<T*>(subsystem);
	}
	return nullptr;
}
