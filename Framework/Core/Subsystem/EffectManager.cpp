#include "stdafx.h"
#include "EffectManager.h"
#include "../../Resource/Effect.h"

EffectManager::EffectManager(Context * context) : ISubsystem(context)
{
}

EffectManager::~EffectManager()
{
	for (auto effect : effects) SAFE_DELETE(effect); //resourceManager사용하면 여기서 터짐
	effects.clear();
	effects.shrink_to_fit();
}

void EffectManager::Initialize()
{
}

void EffectManager::AddEffect(const string & textureFilePath, const string & animationFilePath, const Vector3 & position, const Vector3 & scale)
{
	//auto resourceMgr = context->GetSubsystem<ResourceManager>(); //resourceManager에서 복사 생성자 만드는 것이 더 좋음.
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

	//	if (!effects[i]->IsExist()) effects.erase(effects.begin() + i); //resourceManger가 관리하므로 erase하면서 delete같이함
	//	else i++; //지우면 그 공간이 사라지고 이터레이터는 그대로 가리키므로 한칸 건너 뛰게 됨. 그래서 지우면 i증가 안함
	//}

	for (auto iter = effects.begin(); iter != effects.end();) { //resourceManager등록 안했다면. 사용 안하면 erase전에 delete해야함
		auto effect = *iter;
		if (!effect->IsExist()) {
			SAFE_DELETE(effect);
			iter = effects.erase(iter); //iter -> 지우고 그 자리를 반환
		}
		else iter++;
	} //동적할당이 빈번하면 빈공간이 생기고 그 공간을 활용하지 못하는 병목현상이 생길 수 있음
	//따라서 미리 데이터공간을 할당하는 데이터풀링이 필요함
}

void EffectManager::Render()
{
	for (auto effect : effects) effect->Render();
}
