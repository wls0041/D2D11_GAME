#include "stdafx.h"
#include "Window.h"

function<void(const uint&, const uint&)> Window::OnResize = nullptr;

LRESULT CALLBACK WndProc
(
	HWND handle,
	uint message,
	WPARAM wParam,
	LPARAM lParam
)
{
	if (Input::MouseProc != nullptr)
		Input::MouseProc(message, wParam, lParam);

	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	case WM_SIZE:
		if (Window::OnResize != nullptr && wParam != SIZE_MINIMIZED)
			Window::OnResize(lParam & 0xffff, (lParam >> 16) & 0xffff);
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}

Window::Window()
{
}

Window::~Window()
{
	if (Settings::Get().GetIsFullScreen() == true)
		ChangeDisplaySettings(nullptr, 0);

	DestroyWindow(Settings::Get().GetWindowHandle());
	UnregisterClass(Settings::Get().GetAppName().c_str(), Settings::Get().GetWindowInstance());
}

void Window::Initialize()
{
	//1. 윈도우 클래스 등록
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hInstance = Settings::Get().GetWindowInstance();
	wndClass.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	wndClass.lpszClassName = Settings::Get().GetAppName().c_str();
	wndClass.lpszMenuName = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (Settings::Get().GetIsFullScreen())
	{
		DEVMODE devMode;
		ZeroMemory(&devMode, sizeof(DEVMODE));

		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = static_cast<DWORD>(Settings::Get().GetWidth());
		devMode.dmPelsHeight = static_cast<DWORD>(Settings::Get().GetHeight());
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	DWORD option = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (Settings::Get().GetIsFullScreen() == true)
		option |= WS_POPUP;
	else
		option |= WS_OVERLAPPEDWINDOW;

	//2. 윈도우 생성
	HWND handle = CreateWindowEx
	(
		WS_EX_APPWINDOW,
		Settings::Get().GetAppName().c_str(),//클래스명
		Settings::Get().GetAppName().c_str(),//타이틀명
		option,
		CW_USEDEFAULT,//x
		CW_USEDEFAULT,//y
		CW_USEDEFAULT,//width
		CW_USEDEFAULT,//height
		nullptr,
		nullptr,
		Settings::Get().GetWindowInstance(),
		nullptr
	);
	assert(handle != nullptr);
	Settings::Get().SetWindowHandle(handle);

	//3. 윈도우 보여주기
	RECT rect;
	ZeroMemory(&rect, sizeof(RECT));

	rect.left = 0;
	rect.top = 0;
	rect.right = static_cast<LONG>(Settings::Get().GetWidth());
	rect.bottom = static_cast<LONG>(Settings::Get().GetHeight());

	uint centerX = static_cast<uint>((GetSystemMetrics(SM_CXSCREEN) - Settings::Get().GetWidth()) * 0.5f);
	uint centerY = static_cast<uint>((GetSystemMetrics(SM_CYSCREEN) - Settings::Get().GetHeight()) * 0.5f);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		handle,
		centerX,
		centerY,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		TRUE
	);
	SetForegroundWindow(handle);
	SetFocus(handle);
	ShowCursor(TRUE);

	ShowWindow(handle, SW_SHOWNORMAL);
	UpdateWindow(handle);
}