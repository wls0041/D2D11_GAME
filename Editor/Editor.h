#pragma once

class Editor final
{
public: 
	static function<LRESULT(HWND, uint, WPARAM, LPARAM)> EditorProc; //���� message ó����� window�̹Ƿ� ������ �������� �ٷ� ������ �� �ֵ��� ��

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