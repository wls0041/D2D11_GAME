#include "stdafx.h"
#include "ColliderManager.h"
#include "../../Scene/Component/Collider.h"

ColliderManager::ColliderManager(Context * context) : ISubsystem(context)
{
}

ColliderManager::~ColliderManager()
{
	Clear();
}

void ColliderManager::Initialize()
{
}

void ColliderManager::RegisterCollider(const string & layer, Collider * collider)
{
	colliderLayers[layer].emplace_back(collider);
}

void ColliderManager::RegisterColliders(const string & layer, vector<class Collider*>& colliders)
{
	colliderLayers[layer].insert(colliderLayers[layer].end(), colliders.begin(), colliders.end());
}

void ColliderManager::Clear()
{
	//객체를 가져다 쓰기만 하므로 delete는 하지 않음
	for (auto &layer : colliderLayers) {
		layer.second.clear();
		layer.second.shrink_to_fit();
	}
	colliderLayers.clear();
}

void ColliderManager::HitCheck_AABB(const string & attacker, const string & receiver, const int & caseNum)
{
	for (auto receiverCollider : colliderLayers[receiver])
		for (auto attackerCollider : colliderLayers[attacker]) {
			bool bCheck = receiverCollider->AABB(attackerCollider, caseNum);

			if (bCheck) {
				if (receiverCollider->Event != nullptr) 
					receiverCollider->Event();
			}
		}
}
