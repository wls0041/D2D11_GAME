#include "stdafx.h"
#include "./Framework/Core/Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdParam, int nCmdShow) //������
{
	Window::Create(hInstance, L"D2DGame", 1280, 720);
	Window::Show();

	while (Window::Update())//������ �޼����� ������ ������ �ƴ϶�� ��� ����
	{

	}

	Window::Destroy();

	return 0;
}