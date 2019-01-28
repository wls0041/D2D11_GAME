#pragma once

class Transform
{
public:
	Transform(class Context *context);
	virtual ~Transform();

	const Matrix &GetLocalMatrix() const { return local; } //&자기자신의 공간이 나간다. 무조건 class에 포함되어 있는 변수여야 함
	const Matrix &GetWorldMatrix() const { return world; }
	const Matrix GetWorldRotationMatrix();

	const Vector3 &GetLocalScale() const { return localScale; } 
	const Vector3 &GetLocalRotation() const { return localRotation; }
	const Vector3 &GetLocalPosition() const { return localPosition; }

	const Vector3 GetScale() { return world.GetScale(); } //world, 자기가 부모라면 local과 같음. 자식이라면 다름.
	const Vector3 GetRotation() { return world.GetRotation(); } 
	const Vector3 GetPosition() { return world.GetTranslation(); }

	void SetLocalScale(const Vector3 &localScale);
	void SetLocalRotation(const Vector3 &localRotation);
	void SetLocalPosition(const Vector3 &localPosition);

	Transform *GetParent() const { return parent; }
	Transform *GetRoot() { return HasParent() ? GetParent()->GetRoot() : this; }

	void SetScale(const Vector3 &scale); //world, world가 곱해진 scale이 들어옴, 자식의 scale만 늘리면 parent를 침범할 수 있음. parent의 변화까지 적용된 scale이 필요
	void SetRotation(const Vector3 &rotation);
	void SetPosition(const Vector3 &position);

	void SetParent(Transform *newParent);
	void AddChild(Transform *child);

	const bool HasParent() const { return parent ? true : false; } //class가 변수를 가지고 있지 않음. 지역변수이기 때문에 만들고 나가면 사라지는데 그 공간을 참조하게 되므로 &안붙음 
	const bool HasChild() const { return !childs.empty(); } //empty->비어있으면 true
	void UpdateTransform();

private:
	class Context *context;

	Vector3 localScale;
	Vector3 localRotation;
	Vector3 localPosition;

	//local은 자기 공간기준으로 떨어져 있는 팔.
	//parent를 적용하면 몸에 팔이 붙고 붙은 위치(world)
	Matrix local;
	Matrix world;

	Transform *parent; //부모 자식 관계를 설정해 부모를 따라 자식도 움직이도록 함
	vector<Transform*> childs;
};