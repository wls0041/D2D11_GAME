#pragma once

//�̱���. handle, instance���� ��Ҹ� �����ϴ� class
class Settings
{
public:
	static Settings &Get() { //�ѹ��� ����, ��� class���� ��� �����ؾ��� -> �̱���
		static Settings instance;
		return instance;
	}

	const wstring &GetAppName() const { return appName; }
	HINSTANCE GetWindowInstance() const { return hInstance; }
	HWND GetWindowHandle() const { return handle; }
	const float &GetWidth() const { return width; }
	const float &GetHeight() const { return height; }

	void SetAppName(const wstring &appName) { this->appName = appName; }
	void SetWindowInstance(HINSTANCE hInstance) { this->hInstance = hInstance; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }
	void SetWidth(const float &width) { this->width = width; }
	void SetHeight(const float &height) { this->height = height; }

private:
	Settings() : appName(L""), hInstance(nullptr), handle(nullptr), width(0), height(0) {}

private:
	wstring appName;
	HINSTANCE hInstance;
	HWND handle;
	float width;
	float height;
};