#pragma once

struct Geometry;
struct Shader;
struct Framebuffer;
struct Texture;

#include <glm\fwd.hpp>

void s0_draw(const Framebuffer &f,
			 const Shader &s,
			 const Geometry &g);

void clearFramebuffer(const Framebuffer &f, bool color = true, 
											bool depth = true);

enum RenderFlag {NONE = 0, DEPTH = 1};
void setFlags(int flags);



void setUniform(const Shader &s, int location, float value);
void setUniform(const Shader &s, int location, int value);
void setUniform(const Shader &s, int location, const Texture &value, unsigned slot);


// These will be called each time the variadic unpacking recursion takes place
// Based upon what uniforms are passed in, the correct function will automagically
// be called. In-so-doing, the appropriate glProgramUniform function will also be called.
namespace __internal
{
void t_setUniform(const Shader &s, int &loc_io, int &tex_io, float val);
void t_setUniform(const Shader &s, int &loc_io, int &tex_io, int val);
void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Texture &val);

void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::vec3 &val);
void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::vec4 &val);
void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::mat4 &val);
}

// The recursive template function.
// U is a variadic template parameter. Each time the function gets called,
// the T parameter (val) will consume 1 element of the variadic.
// So if there is recursion (function calling itself), the uniform
// will slowly be eaten by the T parameter.
template<typename T, typename ...U>
void setUniforms(const Shader &s, int &loc_io, int &tex_io,
										const T &val, U &&... uniforms)
{
	__internal::t_setUniform(s, loc_io, tex_io, val);

	// note that, the T parameter isn't a part of this call
	// it instead is stealing a value from the uniforms variadic
	setUniforms(s, loc_io, tex_io, uniforms...);
}

// This base case, is the last function that gets called. Since T is always eating
// values from U, eventually there will be no more U. To ensure that we have a valid function,
// we need to have a base-case like so:
template<typename T>
void setUniforms(const Shader &s, int &loc_io, int &tex_io, const T &val)
{
	__internal::t_setUniform(s, loc_io, tex_io, val);
}


