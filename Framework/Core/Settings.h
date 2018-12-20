#pragma once

//싱글톤. handle, instance등의 요소를 보관하는 class
class Settings
{
public:
	static Settings &Get() { //한번만 생성, 모든 class에서 사용 가능해야함 -> 싱글톤
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