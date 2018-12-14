#pragma once

class App //���� �����Լ�. ��üȭ �Ұ���
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
	//�⺻ ������ �Ǵ� �� ������ ����
	class Window *window;
	class Graphics *graphics;//����ؼ� ���̱� ������ stdafx�� �߰�
};