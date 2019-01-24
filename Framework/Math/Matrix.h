#pragma once

class Matrix final
{
public:
	static const Matrix Identity;

public:
	static const Matrix Scaling(const float &x, const float &y, const float &z);
	static const Matrix Scaling(const float &s);
	static const Matrix Scaling(const class Vector3 &s);

	static const Matrix RotationX(const float &radian);
	static const Matrix RotationY(const float &radian);
	static const Matrix RotationZ(const float &radian);
	static const Matrix RotationYawPitchRoll(const float &yaw, const float &pitch, const float &roll);
	static const Matrix RotationYawPitchRoll(const class Vector3 &rotation);

	static const Matrix Translation(const float &x, const float &y, const float &z);
	static const Matrix Translation(const float &p);
	static const Matrix Translation(const class Vector3 &p);

	static const Matrix LookAtLH(const class Vector3 &eye, const class Vector3 &target, const class Vector3 &up);//view
	static const Matrix OrthoLH(const float &w, const float &h, const float &zn, const float &zf); //projection
	static const Matrix OrthoOffCenterLH(const float &l, const float &r, const float &b, const float &t, const float &zn, const float &zf);
	
	static Matrix Inverse(const Matrix &rhs); //�����
	static Matrix TransPose(const Matrix &rhs); //��ġ���

public:
	Matrix();
	Matrix( //��켱���� �Է��ϸ� �˾Ƽ� ���켱�� �־���
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44
	);
	~Matrix() {}

	operator float *() { return &_11; } //�Ͻ��� ��ȯ method
	operator const float *() const { return &_11; }

	const Vector3 GetScale();
	const Vector3 GetRotation();
	const Vector3 GetTranslation();

	const Matrix Inverse() const { return Inverse(*this); }
	const Matrix TransPose() const { return TransPose(*this); }//��ȯ:��ġ, ����: �״��
	void TransPose() { *this = TransPose(*this); } //���� -> ��ġ

	void Decompose(Vector3 &scale, Vector3 &rotation, Vector3 &translation);

	void SetIdentity();

	const float *Data() const { return &_11; } //class, struct�� ������ ������ �޾� �迭ó�� �����Ǿ� ���� -> ù ���� �ּҸ� �˷��ָ� �� ���� ��ġ�� �� �� ����
	const Matrix operator*(const Matrix &rhs) const;
	const bool operator==(const Matrix &rhs) const;
	const bool operator!=(const Matrix &rhs) const { return !(*this == rhs); }

public:
	float _11, _21, _31, _41; //���켱
	float _12, _22, _32, _42;
	float _13, _23, _33, _43;
	float _14, _24, _34, _44;
};