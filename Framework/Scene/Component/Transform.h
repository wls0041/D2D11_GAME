#pragma once

class Transform
{
public:
	Transform(class Context *context);
	virtual ~Transform();

	const Matrix &GetLocalMatrix() const { return local; } //&�ڱ��ڽ��� ������ ������. ������ class�� ���ԵǾ� �ִ� �������� ��
	const Matrix &GetWorldMatrix() const { return world; }
	const Matrix GetWorldRotationMatrix();

	const Vector3 &GetLocalScale() const { return localScale; } 
	const Vector3 &GetLocalRotation() const { return localRotation; }
	const Vector3 &GetLocalPosition() const { return localPosition; }

	const Vector3 GetScale() { return world.GetScale(); } //world, �ڱⰡ �θ��� local�� ����. �ڽ��̶�� �ٸ�.
	const Vector3 GetRotation() { return world.GetRotation(); } 
	const Vector3 GetPosition() { return world.GetTranslation(); }

	void SetLocalScale(const Vector3 &localScale);
	void SetLocalRotation(const Vector3 &localRotation);
	void SetLocalPosition(const Vector3 &localPosition);

	Transform *GetParent() const { return parent; }
	Transform *GetRoot() { return HasParent() ? GetParent()->GetRoot() : this; }

	void SetScale(const Vector3 &scale); //world, world�� ������ scale�� ����, �ڽ��� scale�� �ø��� parent�� ħ���� �� ����. parent�� ��ȭ���� ����� scale�� �ʿ�
	void SetRotation(const Vector3 &rotation);
	void SetPosition(const Vector3 &position);

	void SetParent(Transform *newParent);
	void AddChild(Transform *child);

	const bool HasParent() const { return parent ? true : false; } //class�� ������ ������ ���� ����. ���������̱� ������ ����� ������ ������µ� �� ������ �����ϰ� �ǹǷ� &�Ⱥ��� 
	const bool HasChild() const { return !childs.empty(); } //empty->��������� true
	void UpdateTransform();

private:
	class Context *context;

	Vector3 localScale;
	Vector3 localRotation;
	Vector3 localPosition;

	//local�� �ڱ� ������������ ������ �ִ� ��.
	//parent�� �����ϸ� ���� ���� �ٰ� ���� ��ġ(world)
	Matrix local;
	Matrix world;

	Transform *parent; //�θ� �ڽ� ���踦 ������ �θ� ���� �ڽĵ� �����̵��� ��
	vector<Transform*> childs;
};