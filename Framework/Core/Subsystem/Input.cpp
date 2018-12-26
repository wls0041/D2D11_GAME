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
	//1번째 인자(함수)와 나머지 인자(함수의 매개변수)를 묶어준 함수를 만듬
	MouseProc = bind(&Input::MsgProc, this, placeholders::_1, placeholders::_2, placeholders::_3);
}

Input::~Input()
{
}

LRESULT Input::MsgProc(const uint & message, const WPARAM & wParam, const LPARAM & lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE) //좌클릭 or 마우스 이동 시 좌표값 갱신
	{
		mousePosition.x = static_cast<float>(LOWORD(lParam)); 
		mousePosition.y = static_cast<float>(HIWORD(lParam)); 
	}

	if (message == WM_MOUSEWHEEL) //마우스 휠 이용시 변경 전 휠 상태를 저장하고 갱신
	{
		short tWheelValue = static_cast<short>(HIWORD(wParam));

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += static_cast<float>(tWheelValue);
	}

	return TRUE;
}

void Input::Initialize()
{
	//키보드 변수 초기화
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
	//마우스 변수 초기화(변수의 데이터 크기 * 8byte)
	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	timeDblClk = GetDoubleClickTime(); //더블클릭시간을 가져옴
	startDblClk[0] = GetTickCount(); //실행시간으로 더블클릭변수[0] 초기화

	for (int i = 1; i < MAX_INPUT_MOUSE; i++) //더블클릭변수의 나머지 초기화
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0); //휠을 돌릴 때 스크롤할 줄 수 를 가져옴
}

void Input::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState)); //키값을 복사하여 저장

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));

	GetKeyboardState(keyState); //keystate에 가상 키 복사(256)

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++) //가상 키 수만큼 반복
	{
		byte key = keyState[i] & 0x80; //각 키를 비트연산
		keyState[i] = key ? 1 : 0; //key가 참이면 1, 아니면 0

		int oldState = keyOldState[i];
		int state = keyState[i];

		//이전 값과 현재 값 비교
		if (oldState == 0 && state == 1) //한 번 누름
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_DOWN); //이전 0, 현재 1 - KeyDown
		else if (oldState == 1 && state == 0) //눌렀다 뗌
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_UP); //이전 1, 현재 0 - KeyUp
		else if (oldState == 1 && state == 1) //누르고 있음
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_PRESS); //이전 1, 현재 1 - KeyPress
		else //아무 입력 없음
			keyMap[i] = static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_NONE);
	}

	memcpy(buttonOldStatus, buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE); //버튼상태 복사하여 저장

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	//마우스 입력에 따라 반환
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0; //좌클릭
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0; //우클릭
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0; //휠

	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1) //클릭
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DOWN);
		else if (tOldStatus == 1 && tStatus == 0) //클릭 후 뗌
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_UP);
		else if (tOldStatus == 1 && tStatus == 1) //클릭 유지
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_PRESS);
		else //입력 없음
			buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_NONE);
	}

	POINT point = { 0, 0 };
	GetCursorPos(&point); //전체화면 기준 마우스의 위치
	ScreenToClient(Settings::Get().GetWindowHandle(), &point); //윈도우핸들 좌표로 변환

	//현재 휠 상태 저장 x, y
	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	//휠 상태 초기화
	wheelStatus.x = static_cast<float>(point.x);
	wheelStatus.y = static_cast<float>(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus; //휠 이동 값
	wheelOldStatus.z = wheelStatus.z; //휠 상태 저장 z

	DWORD tButtonStatus = GetTickCount(); //시간값을 받아 옴
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonMap[i] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DOWN)) //버튼을 누름
		{
			if (buttonCount[i] == 1) //버튼을 누름
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) //일정 시간이 지나면 더블클릭 무효
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1) //원클릭이면
				startDblClk[i] = tButtonStatus;//더블클릭 초기화
		}

		if (buttonMap[i] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_UP))//버튼을 뗌
		{
			if (buttonCount[i] == 1) //원클릭
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk) //더블클릭 지연시 무효
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)//더블클릭
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk) //더블클릭 유효
					buttonMap[i] = static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DBLCLK); //해당내용 저장

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}
