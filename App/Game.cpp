#include "stdafx.h"
#include "Game.h"
#include "./Framework/Rendering/Rect.h"

void Game::Initialize()
{
	timer = context->GetSubsystem<Timer>();
	input = context->GetSubsystem<Input>();
	sceneMgr = context->GetSubsystem<SceneManager>();
}

void Game::Update()
{
	timer->Update();
	input->Update();
	sceneMgr->Update();
}

void Game::Render()
{
	sceneMgr->Render();
}

void Game::Destroy()
{
}
