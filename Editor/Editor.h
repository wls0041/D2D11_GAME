#pragma once

class Editor final
{
public: 
	static function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc; //메인 message 처리기는 window이므로 별도의 과정없이 바로 연동할 수 있도록 함

public:
	Editor();
	~Editor();

	LRESULT MessageProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam); //LRESULT --> static function --> Window.h
	void Resize();
	
	void Initialize(class Context *context);
	void Render();

private:
	void BegineDockspace();
	void EndDockspace();

private:
	class Context *context;
	vector<class IWidget*> widgets;
	bool bInitialized;
	bool bDockspace; 
};