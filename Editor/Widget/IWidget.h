#pragma once
#include "stdafx.h"
#include "../ImGui/Source/imgui.h"

class IWidget
{
public:
	//nocollapse->�ּ�ȭ ���� ���ڴ�
	IWidget(class Context *context) : context(context), title(""), xMin(0.0f), xMax(FLT_MAX), yMin(0.0f), yMax(FLT_MAX), height(0.0f), windowFlags(ImGuiWindowFlags_NoCollapse), bVisible(true)
	{}
	virtual ~IWidget() {}

	//��׵��� �׻� Begin end�� ������ �ְ� �� ���̿��� �ڵ��� ��
	virtual void Begin() {//����
		ImGui::SetNextWindowSize(ImVec2(xMin, yMin), ImGuiCond_FirstUseEver); //���� ó������ ȣ��, �ι�°���ʹ� ������ ������ ȣ��. (����ڰ� �����ϸ� �װ� ����ϵ���, ����� ���� ������ �⺻��)
		ImGui::SetNextWindowSizeConstraints(ImVec2(xMin, yMin), ImVec2(xMax, yMax)); //�� �� ������ ���� ���ϸ� �� �̻����� ���� �þ�� ����
		ImGui::Begin(title.c_str(), &bVisible, windowFlags);
	}
	virtual void Render() = 0; //��� �׸��� �𸣱� ������ ������������ ����
	virtual void End() {//����
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