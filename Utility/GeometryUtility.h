#pragma once

class GeometryUtility
{
public:
	static void CreateQuad(Geometry<VertexColor> &geometry);
	static void CreateQuad(Geometry<VertexTexture> &geometry);
	static void CreateCircle(Geometry<VertexColor>& geometry);

private:

};