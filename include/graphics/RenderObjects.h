#pragma once

//declare our vertex
struct Vertex;

struct Geometry
{
	unsigned handle, // Vertex Array Object
			 vbo,	 // Vertex buffer
			 ibo,	 // Index buffer
			size;	 // How many indices make up this model
};


Geometry makeGeometry(const Vertex *verts, size_t vsize,
					  const unsigned *idxs, size_t isize);

void freeGeometry(Geometry &g);

void solveTangents(Vertex *v, size_t vsize,
					const unsigned *idxs, size_t isize);



struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vert_src, const char *frag_src);
void freeShader(Shader &s);




struct Texture
{
	unsigned handle;
};

// RGBA = 4 channels
// 512x512 image = 262144 pixels * 4 channels = ~1 million
Texture makeTexture(unsigned w, unsigned h, unsigned c, const void *pixels, bool isFloat = false);

void freeTexture(Texture &t);



struct Framebuffer
{
	unsigned handle, width, height, nTargets;
	Texture depthTarget;
	Texture targets[8];
};

Framebuffer makeFramebuffer(unsigned w, unsigned h, unsigned c, bool hasDepth, unsigned nTargets, unsigned nFloatTargets);
