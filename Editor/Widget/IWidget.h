#pragma once
#include "stdafx.h"
#include "../ImGui/Source/imgui.h"

class IWidget
{
public:
	//nocollapse->최소화 없이 쓰겠다
	IWidget(class Context *context) : context(context), title(""), xMin(0.0f), xMax(FLT_MAX), yMin(0.0f), yMax(FLT_MAX), height(0.0f), windowFlags(ImGuiWindowFlags_NoCollapse), bVisible(true)
	{}
	virtual ~IWidget() {}

	//얘네들은 항상 Begin end를 가지고 있고 그 사이에서 코딩을 함
	virtual void Begin() {//가상
		ImGui::SetNextWindowSize(ImVec2(xMin, yMin), ImGuiCond_FirstUseEver); //가장 처음에만 호출, 두번째부터는 변형된 값으로 호출. (사용자가 설정하면 그걸 사용하도록, 저장된 설정 없으면 기본값)
		ImGui::SetNextWindowSizeConstraints(ImVec2(xMin, yMin), ImVec2(xMax, yMax)); //이 안 변수의 값을 정하면 그 이상으로 값이 늘어나지 않음
		ImGui::Begin(title.c_str(), &bVisible, windowFlags);
	}
	virtual void Render() = 0; //어떤걸 그릴지 모르기 때문에 순수가상으로 만듬
	virtual void End() {//가상
		height = ImGui::GetWindowHeight();
		ImGui::End();
	}

	const bool &IsVisible() const { return bVisible; }
	void SetIsVisible(const bool &bVisible) { this->bVisible = bVisible; }

protected:
	class Context *context;
	string title;

	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float height;
	int windowFlags;
	bool bVisible;
};