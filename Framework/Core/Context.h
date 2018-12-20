#pragma once
#include "stdafx.h"
#include "./Subsystem/ISubsystem.h"

//subsystem들을 vector로 관리
class Context final
{
public:
	Context() {}
	~Context() { for (uint i = subsystems.size() - 1; i >= 0; i--) SAFE_DELETE(subsystems[i]); }

	void RegisterSubsystem(class ISubsystem *subsystem) { subsystems.push_back(subsystem); }

	//typename을 T(임의)로 하고 그 타입의 함수를 생성
	template <typename T> //저장된 subsystems의 요소를 수정 삭제할 때 원하는 값을 찾기 위해 template사용
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
