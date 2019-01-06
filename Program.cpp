#include "stdafx.h"
#include "./App/Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdParam, int nCmdShow) //진입점
{
	srand(time(NULL));

	//settings에 값 입력
	Settings::Get().SetAppName(L"D2D11Game");
	Settings::Get().SetWindowInstance(hInstance);
	Settings::Get().SetWidth(1280);
	Settings::Get().SetHeight(720);
	Settings::Get().SetIsVsync(false); //프레임을 일정으로 고정
	Settings::Get().SetIsFullScreen(false);

	//Game 클래스로 넘어가 App이 가진 함수를 이용해 게임 실행 
	Game *game = new Game();
	WPARAM wParam = game->Run();
	SAFE_DELETE(game);
	return wParam;
}