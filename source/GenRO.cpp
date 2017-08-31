
#include "graphics\RenderObjects.h"
#include "graphics\GenRO.h"
#include "graphics\Vertex.h"



Texture makeColor(float r, float g, float b, float a)
{
	unsigned char pixels[] = {r*255, g*255, b*255, a*255};

	return makeTexture(1,1,4,pixels);
}

Geometry makeQuad()
{
	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	return makeGeometry(vquad, 4, quadidx, 6);
}