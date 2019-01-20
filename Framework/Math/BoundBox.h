#pragma once

class BoundBox
{
public:
	static const BoundBox Transformed(const BoundBox &box, const D3DXMATRIX &matrix); //외부용

public:
	BoundBox();
	BoundBox(const D3DXVECTOR3 &minBox, const D3DXVECTOR3 &maxBox);
	BoundBox(const D3DXVECTOR3 &center, const D3DXVECTOR3 &size);
	BoundBox(const vector<VertexTexture> &vertices);
	virtual ~BoundBox();

	const D3DXVECTOR3 &GetMin() const { return minBox; }
	const D3DXVECTOR3 &GetMax() const { return maxBox; }
	const D3DXVECTOR3 GetCenter() const { return (maxBox + minBox) * 0.5f; }
	const D3DXVECTOR3 GetSize() const { return maxBox - minBox; }
	const D3DXVECTOR3 GetExtents() const { return (maxBox - minBox) * 0.5f; }

	Intersection IsInside(const D3DXVECTOR3 &point);
	Intersection IsInside(const BoundBox &box);

	void Transformed(const D3DXMATRIX &matrix); //내부용

	const bool operator == (const BoundBox &rhs) const {
		bool bCheck = true;
		bCheck &= (minBox == rhs.minBox);
		bCheck &= (maxBox == rhs.maxBox);
		return bCheck;
	}

private:
	D3DXVECTOR3	minBox;
	D3DXVECTOR3	maxBox;
};