#pragma once
#include "./IWidget.h"

//��������. ����â�� �޴� ���� �޴� ���� 
class Hierarchy final
{
public:
	Hierarchy(class Context *context);
	~Hierarchy();

	void Render() override;

private:

};