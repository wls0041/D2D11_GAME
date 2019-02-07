#pragma once

enum EngineFlags : unsigned long   //unsinged�� Ȯ��, ����,����÷ο� ���� ����
{
	EngineFlags_Update = 1UL << 0,//ul = unsigned long 1
	EngineFlags_Render = 1UL << 1,//bit�������� ���� �ϳ��� ���� �� ���� 100,
	EngineFlags_Game = 1UL << 2,  // 1001,
};

//context ���� ��� ������ ��� ���� ����
class Engine final
{
public:
	static const unsigned long& GetEngineFlags() { return engineFlags; }//long -> �� ���� ���� ��� ����, static�̱� ������ return �տ� const ������
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
	//�Ʒ� ���� �ǽð����� update�Ǿ�� �ϱ� ������ ����
	class Timer* timer;
	class Input* input;
	class SceneManager* sceneManager;
};