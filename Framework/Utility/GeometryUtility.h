#pragma once

class GeometryUtility
{
public:
	static void CreateQuad(Geometry<VertexColor> &geometry);
	static void CreateQuad(Geometry<VertexTexture> &geometry);

	static void CreateScreenQuad(Geometry<VertexTexture> &geometry);
private:

};