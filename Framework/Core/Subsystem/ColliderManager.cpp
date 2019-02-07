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

void ColliderManager::ClearCollider(const string & name)
{
	if (colliderLayers.count(name) > 0) {
		colliderLayers[name].clear();
		colliderLayers[name].shrink_to_fit();
	}
}

void ColliderManager::HitCheck_AABB(const string & attacker, const string & receiver)
{
	for (auto receiverCollider : colliderLayers[receiver]) {
		for (auto attackerCollider : colliderLayers[attacker]) {
			int bCheck = receiverCollider->AABB(attackerCollider);

			if (bCheck) {
				if (receiverCollider->Event != nullptr)
					receiverCollider->Event();
			}
		}
	}
}

void ColliderManager::HitCheck_AABB_Bullet(const string & attacker, const string & receiver, const CheckCase & caseNum)
{
	for (auto receiverCollider : colliderLayers[receiver]) {
		for (auto attackerCollider : colliderLayers[attacker]) {
			auto bCheck = receiverCollider->AABB_Circle(attackerCollider, caseNum);

			if (bCheck != CircleCheck::None) {
				if (receiverCollider->Event != nullptr) receiverCollider->Event();
				if (attackerCollider->Event != nullptr) attackerCollider->Event();
			}
		}
	}
}

void ColliderManager::HitCheck_AABB_Circle(const string & attacker, const string & receiver, const CheckCase & caseNum)
{
	for (auto receiverCollider : colliderLayers[receiver]) {
		for (auto attackerCollider : colliderLayers[attacker]) {
			auto bCheck = receiverCollider->AABB_Circle(attackerCollider, caseNum);

			if (bCheck != CircleCheck::None) {
				if (receiverCollider->EventCircle != nullptr)
					receiverCollider->EventCircle(bCheck, attackerCollider);
			}
		}
	}
}

void ColliderManager::HitCheck_AABB_Player(const string & attacker, const string & receiver, const float &position)
{
	for (auto receiverCollider : colliderLayers[receiver]) {
		for (auto attackerCollider : colliderLayers[attacker]) {
			auto bCheck = receiverCollider->AABB_Circle(attackerCollider, CheckCase::Circle_Rect);

			if (bCheck != CircleCheck::None) {
				if (receiverCollider->EventPlayer != nullptr)
					receiverCollider->EventPlayer(position);
			}
		}
	}
}

