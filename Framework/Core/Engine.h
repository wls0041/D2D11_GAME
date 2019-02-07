#pragma once

enum EngineFlags : unsigned long   //unsinged로 확장, 오버,언더플로우 위험 있음
{
	EngineFlags_Update = 1UL << 0,//ul = unsigned long 1
	EngineFlags_Render = 1UL << 1,//bit연산으로 셋을 하나로 묶을 수 있음 100,
	EngineFlags_Game = 1UL << 2,  // 1001,
};

//context 등의 모든 정보를 쥐고 있을 것임
class Engine final
{
public:
	static const unsigned long& GetEngineFlags() { return engineFlags; }//long -> 더 많은 공간 사용 위해, static이기 때문에 return 앞에 const 못붙임
	static void SetEngineFlags(const unsigned long& flags) { engineFlags = flags; }
	static void OnEngineFlags(const EngineFlags& flag) { engineFlags |= flag; }
	static void OffEngineFlags(const EngineFlags& flag) { engineFlags &= ~flag; }
	static void ToggleEngineFlags(const EngineFlags& flag) { IsOnEngineFlags(flag) ? OffEngineFlags(flag) : OnEngineFlags(flag); }
	static const bool IsOnEngineFlags(const EngineFlags& flag) { return (engineFlags & flag) > 0UL; }

public:
	Engine();
	~Engine();

	class Context* GetContext() const { return context; }

	void Initialize();
	void Update();

private:
	static unsigned long engineFlags;

	class Context*context;
	//아래 셋은 실시간으로 update되어야 하기 때문에 모음
	class Timer* timer;
	class Input* input;
	class SceneManager* sceneManager;
};