#pragma once

//����, â ����(�����츦 ����ֱ� ���� Ŭ����)
class Window
{
public:
	static function<void(const uint&, const uint&)> OnResize;

public:
	Window();
	virtual ~Window();
	void Initialize();
};