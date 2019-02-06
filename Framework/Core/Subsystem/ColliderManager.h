#pragma once
#include "ISubsystem.h" : public Isubsystem

//collider°ü¸®
class ColliderManager : public ISubsystem
{
public:
	ColliderManager(class Context * context);
	virtual ~ColliderManager();

	void Initialize() override;
	void RegisterCollider(const string &layer, class Collider *collider);
	void RegisterColliders(const string &layer, class vector<class Collider*> &colliders);

	void Clear();
	void ClearCollider(const string &name);
	void HitCheck_AABB(const string &attacker, const string &receiver);
	void HitCheck_AABB_Circle(const string &attacker, const string &receiver, const CheckCase &caseNum);

private:
	map<string, vector<class Collider*>> colliderLayers;
};