#include "stdafx.h"
#include "SceneManager.h"
#include "../../Scene/Scene.h"

SceneManager::SceneManager(Context * context) : ISubsystem(context), currentScene(nullptr), bestScore(0), curBestScore(-1)
{

}

SceneManager::~SceneManager()
{
	//정석 방법
	//map<string, Scene*>::iterator iter = scenes.begin(); 
	//for (; iter != scenes.end(); iter++) SAFE_DELETE(iter->second); //end는 비어있는 공간이므로 !=

	//편법
	for (auto scene : scenes) SAFE_DELETE(scene.second);
}

void SceneManager::Initialize()
{
	RegisterScene("First", new Scene(context));
	SetCurrentScene("First");
}

void SceneManager::Update()
{
	if(currentScene) currentScene->Update();
	if (currentScene->GetRestart()) {
		curBestScore = bestScore;
		scenes.clear();
		Initialize();
	}
	CheckBest();

}

void SceneManager::Render()
{
	if (currentScene) currentScene->Render();
}

void SceneManager::CheckBest()
{
	bestScore = currentScene->GetScore();
	bestScore = Math::clamp(bestScore, curBestScore, 999);
	currentScene->SetBest(bestScore);
	if (bestScore > curBestScore) currentScene->IsBestPlay(true);
}

void SceneManager::SetCurrentScene(const string & name)
{
	assert(scenes.count(name) > 0);
	currentScene = scenes[name];
}

void SceneManager::RegisterScene(const string & name, Scene * scene)
{
	assert(scenes.count(name) < 1);
	scene->SetName(name);
	//1
	//scenes[name] = scene;
	
	//2
	scenes.insert(make_pair(name, scene));
}
