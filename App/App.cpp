#include "stdafx.h"
#include "App.h"
#include "../System/Window.h"

App::App()
{
	window = new Window();
	window->Initialize();

	graphics = new Graphics();
	graphics->Initialize();
}

App::~App()
{
	SAFE_DELETE(graphics);
	SAFE_DELETE(window);
}

WPARAM App::Run() //실행 후 계속해서 도는 루프
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Initialize();

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { //peek.무시하고 지나감, get.기다림
			if (msg.message == WM_QUIT) break;
			
			TranslateMessage(&msg); //특정 message로 가공
			DispatchMessage(&msg); //wndProc으로 msg를 보내줌
		}
		else {
			Update();
			
			graphics->BeginScene(); //..clear
			{
				Render();
			}
			graphics->EndScene();  //..clipping   - double buffering
		}

	}
	Destroy();

	return msg.wParam;
}
