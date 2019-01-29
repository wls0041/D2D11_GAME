#include "stdafx.h"
#include "BoundBox.h"

const BoundBox BoundBox::Transformed(const BoundBox &box, const Matrix &matrix)
{
	Vector3 newCenter = Vector3::TransformCoord(box.GetCenter(), matrix);
	Vector3 oldEdge = box.GetExtents();
	Vector3 newEdge = Vector3(
		Math::Abs(matrix._11) * oldEdge.x + Math::Abs(matrix._21) * oldEdge.y + Math::Abs(matrix._31) * oldEdge.z,
		Math::Abs(matrix._12) * oldEdge.x + Math::Abs(matrix._22) * oldEdge.y + Math::Abs(matrix._32) * oldEdge.z,
		Math::Abs(matrix._13) * oldEdge.x + Math::Abs(matrix._23) * oldEdge.y + Math::Abs(matrix._33) * oldEdge.z
	);

	return BoundBox(newCenter - newEdge, newCenter + newEdge);
}

BoundBox::BoundBox() : isCircle(false)
{
	minBox = Vector3(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
	maxBox = Vector3(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());
}

BoundBox::BoundBox(const Vector3 &minBox, const Vector3 &maxBox) : minBox(minBox), maxBox(maxBox), isCircle(false)
{
}

BoundBox::BoundBox(const vector<VertexTexture>& vertices) : isCircle(false)
{
	minBox = Vector3(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
	maxBox = Vector3(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());

	for (auto vertex : vertices) {
		minBox.x = Math::Min(minBox.x, vertex.position.x);
		minBox.y = Math::Min(minBox.y, vertex.position.y);
		minBox.z = Math::Min(minBox.z, vertex.position.z);

		maxBox.x = Math::Max(maxBox.x, vertex.position.x);
		maxBox.y = Math::Max(maxBox.y, vertex.position.y);
		maxBox.z = Math::Max(maxBox.z, vertex.position.z);
	}
}

BoundBox::~BoundBox()
{
}

Intersection BoundBox::IsInside(const Vector3 & point)
{
	if (point.x < minBox.x || point.x > maxBox.x ||
		point.y < minBox.y || point.y > maxBox.y ||
		point.z < minBox.z || point.z > maxBox.z)
		return Intersection::Outside;

	return Intersection::Inside;
}

Intersection BoundBox::IsInside(const BoundBox & box)
{
	if (!isCircle) {
		if (box.maxBox.x < minBox.x || box.minBox.x > maxBox.x ||
			box.maxBox.y < minBox.y || box.minBox.y > maxBox.y ||
			box.maxBox.z < minBox.z || box.minBox.z > maxBox.z)
			return Intersection::Outside;
		else if (box.minBox.x < minBox.x || box.maxBox.x > maxBox.x ||
			box.minBox.y < minBox.y || box.maxBox.y > maxBox.y ||
			box.minBox.z < minBox.z || box.maxBox.z > maxBox.z)
			return Intersection::Intersect;
		else return Intersection::Inside;
	}
	else if (isCircle) { //자기가 원일 때(타원 x), intersect여부만 판단
		Vector3 radius = GetExtents();
		Vector3 center = GetCenter();

		//원의 중점과 rect의 정점을 비교해 x축이 겹치거나 y축이 겹칠 때 계산(계산량 감소용)
		if ((box.minBox.x <= center.x && box.maxBox.x >= center.x) || (box.minBox.y <= center.y && box.maxBox.y >= center.y)) {
			Vector3 exMaxBox = box.maxBox + radius;
			Vector3 exMinBox = box.minBox - radius;

			if (exMinBox.x < center.x && exMaxBox.x > center.x && exMinBox.y < center.y && exMaxBox.y > center.y) 
				return Intersection::Intersect;
		}
		else if (Vector3::Distance(center, minBox) > radius.x) return Intersection::Intersect; //좌하단
		else if (Vector3::Distance(center, maxBox) > radius.x) return Intersection::Intersect; //우상단
		else if (Vector3::Distance(center, { maxBox.x, minBox.y, 0 }) > radius.x) return Intersection::Intersect; //우하단
		else if (Vector3::Distance(center, { minBox.x, maxBox.y, 0 }) > radius.x) return Intersection::Intersect; //좌하단

		return Intersection::Outside;
	}
}

Intersection BoundBox::IsCircleInside(const BoundBox & box)
{
	if (box.minBox.x < minBox.x && box.maxBox.x > maxBox.x &&
		box.minBox.y < minBox.y && box.maxBox.y > maxBox.y) return Intersection::Inside;
	else return Intersection::Outside;
}

void BoundBox::Transformed(const Matrix & matrix)
{
	Vector3 newCenter = Vector3::TransformCoord(GetCenter(), matrix); ;

	Vector3 oldEdge = GetExtents();
	Vector3 newEdge = Vector3(
		Math::Abs(matrix._11) * oldEdge.x + Math::Abs(matrix._21) * oldEdge.y + Math::Abs(matrix._31) * oldEdge.z,
		Math::Abs(matrix._12) * oldEdge.x + Math::Abs(matrix._22) * oldEdge.y + Math::Abs(matrix._32) * oldEdge.z,
		Math::Abs(matrix._13) * oldEdge.x + Math::Abs(matrix._23) * oldEdge.y + Math::Abs(matrix._33) * oldEdge.z
	);

	this->minBox = newCenter - newEdge;
	this->maxBox = newCenter + newEdge;
}

void BoundBox::Update(const Vector3 & minBox, const Vector3 & maxBox)
{
	this->minBox = minBox;
	this->maxBox = maxBox;
}

void BoundBox::Update(const vector<VertexTexture>& vertices)
{
	minBox = Vector3(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
	minBox = Vector3(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());

	for (auto vertex : vertices) {
		minBox.x = Math::Min(minBox.x, vertex.position.x);
		minBox.y = Math::Min(minBox.y, vertex.position.y);
		minBox.z = Math::Min(minBox.z, vertex.position.z);

		maxBox.x = Math::Max(maxBox.x, vertex.position.x);
		maxBox.y = Math::Max(maxBox.y, vertex.position.y);
		maxBox.z = Math::Max(maxBox.z, vertex.position.z);
	}
}
