#pragma once
#include "stdafx.h"

template <class T>
class Geometry //템플릿 클래스. 기본 도형을 만들 때 사용. 타입에 따라 텍스쳐를 변경해야 하므로 편하게 템플릿으로 사용
{
public:
	Geometry();
	virtual ~Geometry();

	const uint GetVertexCount() const { return vertices.size(); } //앞. 나가는 변수를 묶음, 뒤. 내부의 변수 변동 묶음
	const uint GetIndexCount() const { return indices.size(); }

	const vector<T> &GetVertices() const { return vertices; }
	const vector<uint> &GetIndices() const { return indices; }

	//특정 구간의 데이터 가져옴, offset부터 count만큼
	void GetVertices(const uint &offset, const uint &count, vector<T> &vertices); 
	void GetIndices(const uint &offset, const uint &count, vector<T> &indices);

	T *GetVertexData() { return &vertices[0]; } //방법1. 시작주소
	uint *GetIndexData() { return indices.data(); }//방법2. 함수

	void Clear();

	//기존 추가(하나만)
	void AddVertex(const T &vertex); //참조는 원본 값이 바뀔 수 있기 때문에 이를 방지하기 위해 const로
	void AddIndex(const uint &index);

	//기존 추가(여러 개)
	void AddVertices(const vector<T> &vertices);
	void AddIndices(const vector<uint> &indices);

	//기존 대체
	void SetVertices(const vector<T> &vertices);
	void SetIndices(const vector<uint> &indices);

private:
	vector<T> vertices; //vertices는 시작주소가 아님. vertices라는 vector(container)안의 vertices[0]이 시작주소
	vector<uint> indices;
};

#include "Geometry.inl"
