#pragma once
#include "stdafx.h"

template <class T>
class Geometry //���ø� Ŭ����. �⺻ ������ ���� �� ���. Ÿ�Կ� ���� �ؽ��ĸ� �����ؾ� �ϹǷ� ���ϰ� ���ø����� ���
{
public:
	Geometry();
	virtual ~Geometry();

	const uint GetVertexCount() const { return vertices.size(); } //��. ������ ������ ����, ��. ������ ���� ���� ����
	const uint GetIndexCount() const { return indices.size(); }

	const vector<T> &GetVertices() const { return vertices; }
	const vector<uint> &GetIndices() const { return indices; }

	//Ư�� ������ ������ ������, offset���� count��ŭ
	void GetVertices(const uint &offset, const uint &count, vector<T> &vertices); 
	void GetIndices(const uint &offset, const uint &count, vector<T> &indices);

	T *GetVertexData() { return &vertices[0]; } //���1. �����ּ�
	uint *GetIndexData() { return indices.data(); }//���2. �Լ�

	void Clear();

	//���� �߰�(�ϳ���)
	void AddVertex(const T &vertex); //������ ���� ���� �ٲ� �� �ֱ� ������ �̸� �����ϱ� ���� const��
	void AddIndex(const uint &index);

	//���� �߰�(���� ��)
	void AddVertices(const vector<T> &vertices);
	void AddIndices(const vector<uint> &indices);

	//���� ��ü
	void SetVertices(const vector<T> &vertices);
	void SetIndices(const vector<uint> &indices);

private:
	vector<T> vertices; //vertices�� �����ּҰ� �ƴ�. vertices��� vector(container)���� vertices[0]�� �����ּ�
	vector<uint> indices;
};

#include "Geometry.inl"
