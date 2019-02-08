#include "stdafx.h"
#include "Editor.h"
#include "./ImGui/Source/imgui.h"
#include "./ImGui/imgui_impl_win32.h"
#include "./ImGui/imgui_impl_dx11.h"

#define DOCKING_ENABLED ImGui::GetIO().ConfigFlags & ImguiConfigFlags_DockingEnable //bit연산에 의해 오른쪽이 켜져있을 때만 true 나옴

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

function<LRESULT(HWND, UINT, WPARAM, LPARAM)> Editor::EditorProc = nullptr; //static은 기본적으로 초기화 되어있어 굳이 null안해줘도 되긴 함

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
	ImGuiIO &io = ImGui::GetIO(); //input ouput에 관한 모든 정보 가지고 있음
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //tab을 누르면 다른 창 선택되는 기능
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //docking기능 가능하게 함. 안켜주면 7줄에서 false반환됨
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcon;
	io.ConfigResizeWindowsFromEdges = true; //전체화면 아닐 때 모서리를 드래그해서 사이즈조절 가능하게 함

	ImGui_ImplWin32_Init(Settings::Get().GetWindowHandle());
	ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetDeviceContext());

	//TODO:

	EditorProc = bind(&Editor::MessageProc, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4); //변수 4개 맵핑
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
