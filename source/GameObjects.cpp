#include "graphics\GameObjects.h"
#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Load.h"

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const SpecGloss & val)
{
	__internal::t_setUniform(s, loc_io, tex_io, val.model);
	__internal::t_setUniform(s, loc_io, tex_io, val.diffuse);
	__internal::t_setUniform(s, loc_io, tex_io, val.specular);
	__internal::t_setUniform(s, loc_io, tex_io, val.normal);
	__internal::t_setUniform(s, loc_io, tex_io, val.gloss);
}

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const Camera & val)
{
	__internal::t_setUniform(s, loc_io, tex_io, val.proj);
	__internal::t_setUniform(s, loc_io, tex_io, val.view);
}

void __internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const DirectionalLight & val)
{
	__internal::t_setUniform(s, loc_io, tex_io, val.getProj());
	__internal::t_setUniform(s, loc_io, tex_io, val.getView());
	__internal::t_setUniform(s, loc_io, tex_io, val.color);
	__internal::t_setUniform(s, loc_io, tex_io, val.intensity);
}


SimplePresetScene::SimplePresetScene()
{
	// floor
	Vertex vquad[] = { { { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } }, { { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } }, { { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } }, { { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } } };

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	go[0].geo = makeGeometry(vquad, 4, quadidx, 6);
	go[0].model = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(2.5, 2.5, 1));
	go[0].diffuse	= loadTexture("../../resources/textures/four_diffuse.tga");
	go[0].specular	= loadTexture("../../resources/textures/four_specular.tga");
	go[0].normal	= loadTexture("../../resources/textures/four_normal.tga");
	go[0].gloss		= 4;

	// Soulspear
	go[1].geo		= loadGeometry("../../resources/models/soulspear.obj");
	go[1].diffuse	= loadTexture("../../resources/textures/soulspear_diffuse.tga");
	go[1].specular	= loadTexture("../../resources/textures/soulspear_specular.tga");
	go[1].normal	= loadTexture("../../resources/textures/soulspear_normal.tga");
	go[1].gloss		= 4;

	// Earth
	go[2].geo      = loadGeometry("../../resources/models/sphere.obj");
	go[2].diffuse  = loadTexture("../../resources/textures/earth_diffuse.jpg");
	go[2].specular = loadTexture("../../resources/textures/earth_specular.jpg");
	go[2].normal   = loadTexture("../../resources/textures/earth_normal.jpg");
	go[2].model    = glm::scale(glm::vec3(2,2,2)) * glm::translate(glm::vec3(2, 1, -1));
	go[2].gloss    = 4;

	// Camera
	cam.view = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	cam.proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);

	// Light 1
	dl[0].center	= glm::vec3(0, 0, 0);
	dl[0].color		= glm::vec4(1,1,0, 1);
	dl[0].direction = glm::normalize(glm::vec3(.8, -.2, -.3));
	dl[0].intensity = 1.0;

	// Light 2
	dl[1].center	= glm::vec3(0,0,0);
	dl[1].color		= glm::vec4(0, 1, 1, 1);
	dl[1].direction = glm::normalize(glm::vec3(-.8, -.2, -.3));
	dl[1].intensity	= 1.0;
}
