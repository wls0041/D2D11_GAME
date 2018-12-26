#include "stdafx.h"
#include "Input.h"

function<LRESULT(const uint&, const WPARAM&, const LPARAM&)> Input::MouseProc = nullptr;

Input::Input(Context * context)
	: ISubsystem(context)
	, mousePosition(0, 0, 0)
	, wheelStatus(0, 0, 0)
	, wheelOldStatus(0, 0, 0)
	, wheelMoveValue(0, 0, 0)
{
	//1��° ����(�Լ�)�� ������ ����(�Լ��� �Ű�����)�� ������ �Լ��� ����
	MouseProc = bind(&Input::MsgProc, this, placeholders::_1, placeholders::_2, placeholders::_3);
}

Input::~Input()
{
}

LRESULT Input::MsgProc(const uint & message, const WPARAM & wParam, const LPARAM & lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE) //��Ŭ�� or ���콺 �̵� �� ��ǥ�� ����
	{
		mousePosition.x = static_cast<float>(LOWORD(lParam)); 
		mousePosition.y = static_cast<float>(HIWORD(lParam)); 
	}

	if (message == WM_MOUSEWHEEL) //���콺 �� �̿�� ���� �� �� ���¸� �����ϰ� ����
	{
		short tWheelValue = static_cast<short>(HIWORD(wParam));

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += static_cast<float>(tWheelValue);
	}

	return TRUE;
}

void Input::Initialize()
{
	//Ű���� ���� �ʱ�ȭ
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
	//���콺 ���� �ʱ�ȭ(������ ������ ũ�� * 8byte)
	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	timeDblClk = GetDoubleClickTime(); //����Ŭ���ð��� ������
	startDblClk[0] = GetTickCount(); //����ð����� ����Ŭ������[0] �ʱ�ȭ

	for (int i = 1; i < MAX_INPUT_MOUSE; i++) //����Ŭ�������� ������ �ʱ�ȭ
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0); //���� ���� �� ��ũ���� �� �� �� ������
}

void Input::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState)); //Ű���� �����Ͽ� ����

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));

	GetKeyboardState(keyState); //keystate�� ���� Ű ����(256)

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++) //���� Ű ����ŭ �ݺ�
	{
		byte key = keyState[i] & 0x80; //�� Ű�� ��Ʈ����
		keyState[i] = key ? 1 : 0; //key�� ���̸� 1, �ƴϸ� 0

		int oldState = keyOldState[i];
		int state = keyState[i];

		//���� ���� ���� �� ��
		if (oldState == 0 && state == 1) //�� �� ����
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_DOWN); //���� 0, ���� 1 - KeyDown
		else if (oldState == 1 && state == 0) //������ ��
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_UP); //���� 1, ���� 0 - KeyUp
		else if (oldState == 1 && state == 1) //������ ����
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_PRESS); //���� 1, ���� 1 - KeyPress
		else //�ƹ� �Է� ����
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_NONE);
	}

	memcpy(buttonOldStatus, buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE); //��ư���� �����Ͽ� ����

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	//���콺 �Է¿� ���� ��ȯ
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0; //��Ŭ��
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0; //��Ŭ��
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0; //��

	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1) //Ŭ��
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DOWN);
		else if (tOldStatus == 1 && tStatus == 0) //Ŭ�� �� ��
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_UP);
		else if (tOldStatus == 1 && tStatus == 1) //Ŭ�� ����
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_PRESS);
		else //�Է� ����
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_NONE);
	}

	POINT point = { 0, 0 };
	GetCursorPos(&point); //��üȭ�� ���� ���콺�� ��ġ
	ScreenToClient(Settings::Get().GetWindowHandle(), &point); //�������ڵ� ��ǥ�� ��ȯ

	//���� �� ���� ���� x, y
	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	//�� ���� �ʱ�ȭ
	wheelStatus.x = static_cast<float>(point.x);
	wheelStatus.y = static_cast<float>(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus; //�� �̵� ��
	wheelOldStatus.z = wheelStatus.z; //�� ���� ���� z

	DWORD tButtonStatus = GetTickCount(); //�ð����� �޾� ��
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonMap[i] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DOWN)) //��ư�� ����
		{
			if (buttonCount[i] == 1) //��ư�� ����
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) //���� �ð��� ������ ����Ŭ�� ��ȿ
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1) //��Ŭ���̸�
				startDblClk[i] = tButtonStatus;//����Ŭ�� �ʱ�ȭ
		}

		if (buttonMap[i] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_UP))//��ư�� ��
		{
			if (buttonCount[i] == 1) //��Ŭ��
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) //����Ŭ�� ������ ��ȿ
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)//����Ŭ��
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk) //����Ŭ�� ��ȿ
					buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DBLCLK); //�ش系�� ����

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}
