#include "stdafx.h"
#include "./Framework/Core/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdParam, int nCmdShow) //진입점
{
	Window::Create(hInstance, L"D2DGame", 1280, 720);
	Window::Show();

	while (Window::Update())//나가는 메세지가 들어오면 나가고 아니라면 계속 루프
	{

	}

	Window::Destroy();

	return 0;
}