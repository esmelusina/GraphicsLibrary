
#define GLM_FORCE_SWIZZLE

#include "glinc.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"



#ifdef _DEBUG
#include <iostream>
#endif





glm::vec4 calcTangent(const Vertex &v0, const Vertex &v1, const Vertex &v2)
{
	glm::vec4 p1 = v1.position - v0.position;
	glm::vec4 p2 = v2.position - v0.position;

	glm::vec2 t1 = v1.texCoord - v0.texCoord;
	glm::vec2 t2 = v2.texCoord - v0.texCoord;

	return glm::normalize((p1*t2.y - p2*t1.y) / (t1.x*t2.y - t1.y*t2.x));
}

//#define GLM_FORCE_SWIZZLE
void solveTangents(Vertex *v, size_t vsize, const unsigned *idxs, size_t isize)
{
	for (int i = 0; i < isize; i += 3)
	{
		glm::vec4 T = calcTangent(v[idxs[i]],v[idxs[i+1]],v[idxs[i+2]]);

		for (int j = 0; j < 3; ++j)
			v[idxs[i + j]].tangent = glm::normalize(T + v[idxs[i + j]].tangent);
	}

	for (int i = 0; i < vsize; ++i)
	v[i].bitangent = glm::vec4(glm::cross(v[i].normal.xyz(),v[i].tangent.xyz()),0);
}



Geometry makeGeometry(const Vertex *verts, size_t vsize,
					  const unsigned *idxs, size_t isize)
{
	Geometry retval = {0,0,0,isize};

	//Declare our openGL objects and acquire handles.
	glGenBuffers(1, &retval.vbo);
	glGenBuffers(1, &retval.ibo);
	glGenVertexArrays(1, &retval.handle);

	// now lets scope the variables!
	glBindVertexArray(retval.handle);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);

	// initialize all of our buffers
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  isize * sizeof(unsigned), idxs, GL_STATIC_DRAW);

	// describe our memory layout
	glEnableVertexAttribArray(0);  // position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);  // color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)16);

	glEnableVertexAttribArray(2);  // texCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)32);

	glEnableVertexAttribArray(3);  // normals
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)40);

	glEnableVertexAttribArray(4);  // tangent
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)56);

	glEnableVertexAttribArray(5);  // bitangent
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
									sizeof(Vertex), (void*)72);

	// unbind the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return retval;
}

void freeGeometry(Geometry &g)
{
	glDeleteBuffers(1, &g.vbo);
	glDeleteBuffers(1, &g.ibo);
	glDeleteVertexArrays(1, &g.handle);
	g = {0,0,0,0};
}




Shader makeShader(const char *vert_src, const char *frag_src)
{
	Shader retval = { 0 };

	retval.handle = glCreateProgram();

	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vert_src, 0);
	glShaderSource(fs, 1, &frag_src, 0);

	glCompileShader(vs);
	glCompileShader(fs);

#ifdef _DEBUG
	GLint success = GL_FALSE;

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(vs, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}

	success = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(fs, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif



	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);

	glLinkProgram(retval.handle);


#ifdef _DEBUG
	success = GL_FALSE;
	glGetProgramiv(retval.handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(retval.handle, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(retval.handle, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif _DEBUG


	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void freeShader(Shader &s)
{
	glDeleteProgram(s.handle);
	s = { 0 };
}








Texture makeTexture(unsigned w, unsigned h, unsigned c,
	const unsigned char *pixels)
{
	Texture retval = { 0 };

	GLenum f = 0;
	switch (c)
	{
	case 1: f = GL_RED;  break;
	case 2: f = GL_RG;   break;
	case 3: f = GL_RGB;  break;
	case 4: f = GL_RGBA; break;
	}

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	return retval;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}