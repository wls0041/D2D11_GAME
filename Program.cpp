#include "stdafx.h"
#include "./App/Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpszCmdParam, int nCmdShow) //������
{
	srand(time(NULL));

	//settings�� �� �Է�
	Settings::Get().SetAppName(L"D2D11Game");
	Settings::Get().SetWindowInstance(hInstance);
	Settings::Get().SetWidth(1280);
	Settings::Get().SetHeight(720);
	Settings::Get().SetIsVsync(false); //�������� �������� ����
	Settings::Get().SetIsFullScreen(false);

	//Game Ŭ������ �Ѿ App�� ���� �Լ��� �̿��� ���� ���� 
	Game *game = new Game();
	WPARAM wParam = game->Run();
	SAFE_DELETE(game);
	return wParam;
}