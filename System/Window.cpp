#include "stdafx.h"
#include "Window.h"

LRESULT CALLBACK WndProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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
	DestroyWindow(Settings::Get().GetWindowHandle());
	UnregisterClassW(
		Settings::Get().GetAppName().c_str(),
		Settings::Get().GetWindowInstance()
	);
}

void Window::Initialize()
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO); //�ּ�ȭ ������
	wndClass.hInstance = Settings::Get().GetWindowInstance();
	wndClass.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	wndClass.lpszClassName = Settings::Get().GetAppName().c_str();
	wndClass.lpszMenuName = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW; //����, ���� �ٽñ׸���
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD check = RegisterClassEx(&wndClass);
	assert(check != 0); //����� ��� �Ǿ��ٸ� 0�ʰ��� ���� ����

	HWND handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		Settings::Get().GetAppName().c_str(),
		Settings::Get().GetAppName().c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<int>(Settings::Get().GetWidth()),
		static_cast<int>(Settings::Get().GetHeight()),
		nullptr,
		nullptr,
		Settings::Get().GetWindowInstance(),
		nullptr
	);
	assert(handle != nullptr);

	Settings::Get().SetWindowHandle(handle);

	ShowWindow(handle, SW_SHOWNORMAL);
	UpdateWindow(handle);
}
