#pragma once
#include "./IWidget.h"

//계층구조. 설정창의 메뉴 안의 메뉴 같은 
class Hierarchy final
{
public:
	Hierarchy(class Context *context);
	~Hierarchy();

	void Render() override;

private:

};