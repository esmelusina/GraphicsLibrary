#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\ext.hpp"
#include "graphics\GenRO.h"


int main()
{
	Context context;
	context.init();

	Geometry quad = makeQuad();

	Geometry ss_geo      = loadGeometry("../../resources/models/soulspear.obj");
	Texture  ss_diffuse  = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	Texture  ss_normal   = loadTexture("../../resources/textures/soulspear_normal.tga");	
	Texture	 ss_specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	float    ss_gloss    = 4.0f;

	Shader phongToBuffer = loadShader("../../resources/shaders/phongmulti.vert",
							 		  "../../resources/shaders/phongmulti.frag");

	Shader quadDraw = loadShader("../../resources/shaders/quaddraw.vert",
								 "../../resources/shaders/quaddraw.frag");

	Framebuffer screen  = { 0, 800, 600 };
	Framebuffer pbuffer = makeFramebuffer(800, 600, 4, true, 3, 1);


	// Camera
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 2, 3),
								     glm::vec3(0, 2, 0), 
								     glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 1.f);

	// Model
	glm::mat4 ss_model;

	// Light Data
	glm::vec3 l_direction = glm::normalize(glm::vec3(1, -1, -1));
	glm::vec4 l_color = glm::vec4(.9,.6,.7,1);
	float     l_intensity = 2.f;
	glm::vec4 l_ambient = glm::vec4(.1,.02,.2,1);


	int loc, tslot;
	while (context.step())
	{
		float time = context.getTime();
		ss_model = glm::rotate(time, glm::vec3(0, 1, 0));

		/////////////////////////////////////////////////////////////////////////////
		//////////// Draw Soul Spear
		clearFramebuffer(pbuffer);
		setFlags(RenderFlag::DEPTH);
		loc = 0, tslot = 0;

		setUniforms(phongToBuffer, loc, tslot, cam_proj, cam_view,	    // Camera
				ss_model, ss_diffuse, ss_normal, ss_specular, ss_gloss, // model
				l_direction, l_color, l_intensity, l_ambient);			// light

		s0_draw(pbuffer, phongToBuffer, ss_geo);

		/////////////////////////////////////////////////////////////////////////////
		//////////// Draw full Screen Texture
		clearFramebuffer(screen, true, false);
		setFlags(RenderFlag::NONE);

		for(int i = 0; i < 4; ++i)
		{ 
			glm::mat4 q_mod =  glm::translate(glm::vec3(-.5 + i%2,-.5 + i/2,0)) * glm::scale(glm::vec3(.5,.5,1));
			loc = 0, tslot = 0;
			setUniforms(quadDraw, loc, tslot, q_mod, pbuffer.depthTarget);
			s0_draw(screen, quadDraw, quad);
		}
		
	}
	context.term();
	return 0;
}