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

WPARAM App::Run() //���� �� ����ؼ� ���� ����
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Initialize();

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { //peek.�����ϰ� ������, get.��ٸ�
			if (msg.message == WM_QUIT) break;
			
			TranslateMessage(&msg); //Ư�� message�� ����
			DispatchMessage(&msg); //wndProc���� msg�� ������
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
