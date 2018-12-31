#include "Geometry.h"

template<class T>
inline Geometry<T>::Geometry()
{
}

template<class T>
inline Geometry<T>::~Geometry()
{
	Clear();
}

template<class T>
inline void Geometry<T>::GetVertices(const uint & offset, const uint & count, vector<T>& vertices)
{
	auto first = this->vertices.begin() + offset;
	auto last = this->vertices.begin() + offset + count;

	vertices.assign(first, last);
}

template<class T>
inline void Geometry<T>::GetIndices(const uint & offset, const uint & count, vector<T>& indices)
{
	auto first = this->indices.begin() + offset;
	auto last = this->indices.begin() + offset + count;

	indices.assign(first, last);
}

template<class T>
inline void Geometry<T>::Clear()
{
	vertices.clear(); //������ ��� clear�ϸ� �޸� ������. �׷��� �������� ��ź�� �� �����̸� delete�ʿ�
	vertices.shrink_to_fit(); //�޸𸮰����� �°� �ٿ���

	indices.clear();
	indices.shrink_to_fit();
}

template<class T>
inline void Geometry<T>::AddVertex(const T & vertex)
{
	vertices.emplace_back(vertex);
}

template<class T>
inline void Geometry<T>::AddIndex(const uint & index)
{
	indices.emplace_back(index);
}

template<class T>
inline void Geometry<T>::AddVertices(const vector<T>& vertices)
{
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());//end�� 7���� ������ ������ �� ����(8��)�� ������ ����Ŵ
}

template<class T>
inline void Geometry<T>::AddIndices(const vector<uint>& indices)
{
	this->indices.insert(this->indices.end(), indices.begin(), indices.end());
}

template<class T>
inline void Geometry<T>::SetVertices(const vector<T>& vertices)
{
	Clear();
	this->vertices.assign(vertices.begin(), vertices.end()); //���1. �Լ�
}

template<class T>
inline void Geometry<T>::SetIndices(const vector<uint>& indices)
{
	Clear();
	this->indices = indices; //���2. ����
}
