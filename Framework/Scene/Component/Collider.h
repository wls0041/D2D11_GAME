#pragma once

class Collider
{
public:
	Collider(class Context *context);
	virtual ~Collider();

	class Transform *GetTransform() const { return transform; }
	const Vector3 &GetCenter() const { return center; }
	const Vector3 &GetSize() const { return size; }
	const BoundBox GetBoundBox() const;

	void SetTransform(class Transform *transform) { this->transform = transform; }
	void SetCenter(const Vector3 &center) { this->center = center; }
	void SetSize(const Vector3 &size) { this->size = size; }

	const bool AABB(Collider* collider);
	const CircleCheck AABB_Circle(Collider *collider, const CheckCase &caseNum);

	void Update();

public:
	function<void()> Event; //�Լ� �����͸� �̿�. ��ϵ� �Լ��� ����
	function<void(const CircleCheck &check, Collider *opponent)> EventCircle; //�Լ� �����͸� �̿�. ��ϵ� �Լ��� ����

private:
	class Context *context;
	class Transform *transform;
	Vector3 center;
	Vector3 size;
};