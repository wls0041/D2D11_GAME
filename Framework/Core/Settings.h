#pragma once

//�̱���. handle, instance���� ��Ҹ� �����ϴ� class
class Settings
{
public:
	static Settings& Get()//�ѹ��� ����, ��� class���� ��� �����ؾ��� -> �̱���
	{
		static Settings instance;
		return instance;
	}

public:
	const wstring& GetAppName() const { return appName; }
	HINSTANCE GetWindowInstance() const { return hInstance; }
	HWND GetWindowHandle() const { return handle; }
	const float& GetWidth() const { return width; }
	const float& GetHeight() const { return height; }
	const bool& GetIsVsync() const { return bVsync; }
	const bool& GetIsFullScreen() const { return bFullScreen; }
	const D3D11_VIEWPORT& GetViewPort() const { return viewport; }

	void SetAppName(const wstring& appName) { this->appName = appName; }
	void SetWindowInstance(HINSTANCE hInstance) { this->hInstance = hInstance; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }
	void SetWidth(const float& width) { this->width = width; }
	void SetHeight(const float& height) { this->height = height; }
	void SetIsVsync(const bool& bVsync) { this->bVsync = bVsync; }
	void SetIsFullScreen(const bool& bFullScreen) { this->bFullScreen = bFullScreen; }
	void SetViewPort(D3D11_VIEWPORT viewport) { this->viewport = viewport; }

private:
	Settings() {}
	virtual ~Settings() {}

private:
	wstring appName;
	HINSTANCE hInstance;
	HWND handle;
	float width;
	float height;
	bool bVsync;
	bool bFullScreen;

	D3D11_VIEWPORT viewport;
};