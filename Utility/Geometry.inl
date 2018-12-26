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
	vertices.clear(); //동적일 경우 clear하면 메모리 누수남. 그래서 동적인지 판탄한 후 동적이면 delete필요
	vertices.shrink_to_fit(); //메모리공간을 맞게 줄여줌

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
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());//end는 7개의 공간이 있으면 그 다음(8번)의 공간을 가리킴
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
	this->vertices.assign(vertices.begin(), vertices.end()); //방법1. 함수
}

template<class T>
inline void Geometry<T>::SetIndices(const vector<uint>& indices)
{
	Clear();
	this->indices = indices; //방법2. 대입
}
