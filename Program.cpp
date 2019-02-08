#include "stdafx.h"
#include "./Framework/Core/Window.h"
#include "./Framework/Core/Engine.h"

static Engine *engine;
static Graphics *graphics;

void Initialize();
void Resize(const uint&, const uint&);
void Destroy();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdParam, int nCmdShow) //진입점
{
	Window::Create(hInstance, L"D2DGame", 1280, 720);
	Window::Show();

	Initialize();

	while (Window::Update())//나가는 메세지가 들어오면 나가고 아니라면 계속 루프
	{
		engine->Update();

		graphics->BeginScene();
		{

		}
		graphics->EndScene();
	}

	Destroy();
	Window::Destroy();

	return 0;
}

void Initialize()
{
	Settings::Get().SetAppName(Window::AppName);
	Settings::Get().SetWindowInstance(Window::Instance);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));
	Settings::Get().SetIsFullScreen(Window::IsFullScreen);
	Settings::Get().SetIsVsync(false);

	engine = new Engine();
	engine->Initialize();

	graphics = engine->GetContext()->GetSubsystem<Graphics>();

	Window::InputProc = Input::MouseProc;
	Window::Resize = Resize;
}

void Resize(const uint &width, const uint &height)
{
}

void Destroy()
{
	SAFE_DELETE(engine);
}
