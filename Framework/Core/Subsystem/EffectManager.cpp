#include "stdafx.h"
#include "EffectManager.h"
#include "../../Resource/Effect.h"

EffectManager::EffectManager(Context * context) : ISubsystem(context)
{
}

EffectManager::~EffectManager()
{
	for (auto effect : effects) SAFE_DELETE(effect); //resourceManager����ϸ� ���⼭ ����
	effects.clear();
	effects.shrink_to_fit();
}

void EffectManager::Initialize()
{
}

void EffectManager::AddEffect(const string & textureFilePath, const string & animationFilePath, const Vector3 & position, const Vector3 & scale)
{
	//auto resourceMgr = context->GetSubsystem<ResourceManager>(); //resourceManager���� ���� ������ ����� ���� �� ����.
	auto effect = new Effect(context);

	effect->SetTexture(textureFilePath);
	effect->SetAnimation(animationFilePath);
	effect->SetPosition(position);
	effect->SetScale(scale);
	effect->SetIsExist(true);

	//resourceMgr->RegisterResource(effect);
	effects.emplace_back(effect);
}

void EffectManager::Update()
{
	//for (uint i = 0; i < effects.size();) {
	//	effects[i]->Update();

	//	if (!effects[i]->IsExist()) effects.erase(effects.begin() + i); //resourceManger�� �����ϹǷ� erase�ϸ鼭 delete������
	//	else i++; //����� �� ������ ������� ���ͷ����ʹ� �״�� ����Ű�Ƿ� ��ĭ �ǳ� �ٰ� ��. �׷��� ����� i���� ����
	//}

	for (auto iter = effects.begin(); iter != effects.end();) { //resourceManager��� ���ߴٸ�. ��� ���ϸ� erase���� delete�ؾ���
		auto effect = *iter;
		if (!effect->IsExist()) {
			SAFE_DELETE(effect);
			iter = effects.erase(iter); //iter -> ����� �� �ڸ��� ��ȯ
		}
		else iter++;
	} //�����Ҵ��� ����ϸ� ������� ����� �� ������ Ȱ������ ���ϴ� ���������� ���� �� ����
	//���� �̸� �����Ͱ����� �Ҵ��ϴ� ������Ǯ���� �ʿ���
}

void EffectManager::Render()
{
	for (auto effect : effects) effect->Render();
}
