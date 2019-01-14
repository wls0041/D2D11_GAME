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

WPARAM App::Run() //���� �� ����ؼ� ���� ����
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Initialize();
	auto graphics = context->GetSubsystem<Graphics>(); //template. �����ϴ� ���� ��� <> �ȿ� ��

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
