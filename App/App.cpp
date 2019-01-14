#include "stdafx.h"
#include "App.h"
#include "../Framework/Core/Window.h"

App::App()
{
	window = new Window();
	window->Initialize();

	context = new Context();
	context->RegisterSubsystem(new Timer(context));
	context->RegisterSubsystem(new Input(context));
	context->RegisterSubsystem(new Audio(context));
	context->RegisterSubsystem(new Graphics(context));
	context->RegisterSubsystem(new ResourceManager(context));
	context->RegisterSubsystem(new SceneManager(context));
}

App::~App()
{
	SAFE_DELETE(context);
	SAFE_DELETE(window);
}

WPARAM App::Run() //실행 후 계속해서 도는 루프
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Initialize();
	auto graphics = context->GetSubsystem<Graphics>(); //template. 세팅하는 것은 모두 <> 안에 들어감

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
