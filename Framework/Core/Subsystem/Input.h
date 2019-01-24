#pragma once
#include "ISubsystem.h"

#define MAX_INPUT_KEY 255
#define MAX_INPUT_MOUSE 8

class Input : public ISubsystem
{
public:
	static function<LRESULT(const uint&, const WPARAM&, const LPARAM&)> MouseProc;

public:
	Input(class Context* context);
	virtual ~Input();

	const Vector3& GetMousePosition() const { return mousePosition; }
	const Vector3& GetMouseMoveValue() const { return wheelMoveValue; }

	LRESULT MsgProc(const uint& message, const WPARAM& wParam, const LPARAM& lParam);

	void Initialize() override;
	void Update();

	const bool BtnDown(const DWORD& button) const { return buttonMap[button] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_DOWN); }
	const bool BtnUp(const DWORD& button) const { return buttonMap[button] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_UP); }
	const bool BtnPress(const DWORD& button) const { return buttonMap[button] == static_cast<uint>(ButtonStatus::BUTTON_INPUT_STATUS_PRESS); }

	const bool KeyDown(IN const DWORD& key) const { return keyMap[key] == static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_DOWN); }
	const bool KeyUp(IN const DWORD& key) const { return keyMap[key] == static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_UP); }
	const bool KeyPress(IN const DWORD& key) const { return keyMap[key] == static_cast<uint>(KeyStatus::KEY_INPUT_STATUS_PRESS); }

private:
	enum class MouseRotationState
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
	};

	enum class ButtonStatus
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

	enum class KeyStatus
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

private:
	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];
	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];
	DWORD timeDblClk;
	Vector3 mousePosition; //마우스 위치
	Vector3 wheelStatus;
	Vector3 wheelOldStatus;
	Vector3 wheelMoveValue;
};