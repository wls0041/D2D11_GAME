#pragma once

class App //순수 가상함수. 실체화 불가능
{
public:
	App();
	virtual ~App();

	virtual WPARAM Run();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;

protected:
	//기본 메인이 되는 두 정보를 가짐
	class Window *window;
	class Context *context;//graphics를 subsystem으로 옮겨서 관리하므로 context로 관리
};