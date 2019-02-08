#include "stdafx.h"
#include "Editor.h"
#include "./ImGui/Source/imgui.h"
#include "./ImGui/imgui_impl_win32.h"
#include "./ImGui/imgui_impl_dx11.h"

#define DOCKING_ENABLED ImGui::GetIO().ConfigFlags & ImguiConfigFlags_DockingEnable //bit���꿡 ���� �������� �������� ���� true ����

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

function<LRESULT(HWND, UINT, WPARAM, LPARAM)> Editor::EditorProc = nullptr; //static�� �⺻������ �ʱ�ȭ �Ǿ��־� ���� null�����൵ �Ǳ� ��

Editor::Editor() : context(nullptr), bInitialized(false), bDockspace(true)
{
}

Editor::~Editor()
{
}

LRESULT Editor::MessageProc(HWND handle, uint message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
}

void Editor::Resize()
{
}

void Editor::Initialize(Context * context)
{
	if (bInitialized) return;

	this->context = context;
	auto graphics = context->GetSubsystem<Graphics>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); //input ouput�� ���� ��� ���� ������ ����
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //tab�� ������ �ٸ� â ���õǴ� ���
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //docking��� �����ϰ� ��. �����ָ� 7�ٿ��� false��ȯ��
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcon;
	io.ConfigResizeWindowsFromEdges = true; //��üȭ�� �ƴ� �� �𼭸��� �巡���ؼ� ���������� �����ϰ� ��

	ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());

	//TODO:

	EditorProc = bind(&Editor::MessageProc, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4); //���� 4�� ����
	bInitialized = true;
	

}

void Editor::Render()
{
}


void Editor::BegineDockspace()
{
}

void Editor::EndDockspace()
{
}
