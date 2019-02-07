#include "stdafx.h"
#include "Engine.h"

unsigned long Engine::engineFlags = 0UL;

Engine::Engine()
	: timer(nullptr)
	, input(nullptr)
	, sceneManager(nullptr)
{
	engineFlags =
		EngineFlags_Update |
		EngineFlags_Render;

	context = new Context();

	context->RegisterSubsystem(new Timer(context));
	context->RegisterSubsystem(new Input(context));
	context->RegisterSubsystem(new Audio(context));
	context->RegisterSubsystem(new Graphics(context));
	context->RegisterSubsystem(new DirectWrite(context));
	context->RegisterSubsystem(new ResourceManager(context));
	context->RegisterSubsystem(new ColliderManager(context));
	context->RegisterSubsystem(new SceneManager(context));
	context->RegisterSubsystem(new EffectManager(context));
}

Engine::~Engine()
{
	SAFE_DELETE(context);
}

void Engine::Initialize()
{
}

void Engine::Update()
{
}
