#pragma once

//세팅, 창 띄우기(윈도우를 띄어주기 위한 클래스)
class Window
{
public:
	static function<void(const uint&, const uint&)> OnResize;

public:
	Window();
	virtual ~Window();
	void Initialize();
};