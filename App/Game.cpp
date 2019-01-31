#include "stdafx.h"
#include "Game.h"
#include "./Framework/Rendering/Rect.h"

void Game::Initialize()
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();
	audio = context->GetSubsystem<Audio>();
	sceneMgr = context->GetSubsystem<SceneManager>();
	effectMgr = context->GetSubsystem<EffectManager>();
}

void Game::Update()
{
	timer->Update();
	input->Update();
	audio->Update();
	effectMgr->Update();
	sceneMgr->Update();
}

void Game::Render()
{
	effectMgr->Render();
	sceneMgr->Render();
}

void Game::Destroy()
{
}
