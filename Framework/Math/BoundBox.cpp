#include "stdafx.h"
#include "BoundBox.h"

const BoundBox BoundBox::Transformed(const BoundBox &box, const D3DXMATRIX &matrix)
{
	D3DXVECTOR3 newCenter = box.GetCenter();
	D3DXVec3TransformCoord(&newCenter, &newCenter, &matrix);
	
	D3DXVECTOR3 oldEdge = box.GetExtents();
	D3DXVECTOR3 newEdge = D3DXVECTOR3(
		Math::Abs(matrix._11) * oldEdge.x + Math::Abs(matrix._21) * oldEdge.y + Math::Abs(matrix._31) * oldEdge.z,
		Math::Abs(matrix._12) * oldEdge.x + Math::Abs(matrix._22) * oldEdge.y + Math::Abs(matrix._32) * oldEdge.z,
		Math::Abs(matrix._13) * oldEdge.x + Math::Abs(matrix._23) * oldEdge.y + Math::Abs(matrix._33) * oldEdge.z
	);

	return BoundBox(newCenter - newEdge, newCenter + newEdge);
}

BoundBox::BoundBox()
{
	minBox = D3DXVECTOR3(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
	minBox = D3DXVECTOR3(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());
}

BoundBox::BoundBox(const D3DXVECTOR3 & minBox, const D3DXVECTOR3 & maxBox) : minBox(minBox), maxBox(maxBox)
{
}

BoundBox::BoundBox(const vector<VertexTexture>& vertices)
{
	minBox = D3DXVECTOR3(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
	minBox = D3DXVECTOR3(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());

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

Intersection BoundBox::IsInside(const D3DXVECTOR3 & point)
{
	if (point.x < minBox.x || point.x > maxBox.x ||
		point.y < minBox.y || point.y > maxBox.y ||
		point.z < minBox.z || point.z > maxBox.z)
		return Intersection::Outside;

	return Intersection::Inside;;
}

Intersection BoundBox::IsInside(const BoundBox & box)
{
	if (box.maxBox.x < minBox.x || box.minBox.x > maxBox.x ||
		box.maxBox.y < minBox.y || box.minBox.y > maxBox.y ||
		box.maxBox.z < minBox.z || box.minBox.z > maxBox.z)
		return Intersection::Outside;
	else if(box.minBox.x < minBox.x || box.maxBox.x > maxBox.x ||
			box.minBox.y < minBox.y || box.maxBox.y > maxBox.y ||
			box.minBox.z < minBox.z || box.maxBox.z > maxBox.z)
		return Intersection::Intersect;
	else return Intersection::Inside;;
}

void BoundBox::Transformed(const D3DXMATRIX & matrix)
{
	D3DXVECTOR3 newCenter =GetCenter();
	D3DXVec3TransformCoord(&newCenter, &newCenter, &matrix);

	D3DXVECTOR3 oldEdge = GetExtents();
	D3DXVECTOR3 newEdge = D3DXVECTOR3(
		Math::Abs(matrix._11) * oldEdge.x + Math::Abs(matrix._21) * oldEdge.y + Math::Abs(matrix._31) * oldEdge.z,
		Math::Abs(matrix._12) * oldEdge.x + Math::Abs(matrix._22) * oldEdge.y + Math::Abs(matrix._32) * oldEdge.z,
		Math::Abs(matrix._13) * oldEdge.x + Math::Abs(matrix._23) * oldEdge.y + Math::Abs(matrix._33) * oldEdge.z
	);

	this->minBox = newCenter - newEdge;
	this->maxBox = newCenter + newEdge;
}
