#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\GameObjects.h"
#include "graphics\RenderObjects.h"
#include "graphics\Load.h"
#include "graphics\Vertex.h"


void main()
{
	Context context;
	context.init(1280, 720);

	SimplePresetScene scene;



	// Fullscreen quad
	Vertex vquad[] = { { { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } } };
	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	/////////////////////////
	// Shaders

	// draw all geometry to gbuffer
	Shader gpass = loadShader("../../resources/shaders/gpass.vert", "../../resources/shaders/gpass.frag");

	// directional lighting shadow/light pass
	Shader spass_dir = loadShader("../../resources/shaders/shadow.vert", "../../resources/shaders/shadow.frag");
	Shader lpass_dir = loadShader("../../resources/shaders/lpass_dir.vert", "../../resources/shaders/lpass_dir.frag");
	
	// composites into the screen
	Shader cpass = loadShader("../../resources/shaders/cpass.vert", "../../resources/shaders/cpass.frag");

	/////////////////////////
	// Buffers
	Framebuffer gbuff = makeFramebuffer(1280, 720, 4, true , 2, 2); // color, spec, pos, normal
	Framebuffer lbuff = makeFramebuffer(1280, 720, 4, false, 2, 0); // color, spec
	Framebuffer sbuff = makeFramebuffer(2048, 2048, 0, true, 0, 0);
	Framebuffer screen = { 0,1280, 720 };


	int loc, slot;
	while (context.step())
	{
		float time = context.getTime();

		//////////////////////////////////
		// GPASS
		setFlags(RenderFlag::DEPTH);
		clearFramebuffer(gbuff);
		for(int i = 0; i < 3; ++i)
		{ 
			loc = slot = 0;
			setUniforms(gpass, loc, slot, scene.cam, scene.go[i]);
			s0_draw(gbuff, gpass, scene.go[i].geo);
		}

		//////////////////////////////////
		// LPASS Directional
		clearFramebuffer(lbuff);
		
		for (int i = 0; i < 2; ++i)
		{
			setFlags(RenderFlag::DEPTH);
			clearFramebuffer(sbuff);
			for(int j = 0; j < 3; ++j)
			{
				loc = slot = 0;
				setUniforms(spass_dir, loc, slot, scene.dl[i].getProj(), scene.dl[i].getView());
				s0_draw(sbuff, spass_dir, scene.go[j].geo);
			}
			setFlags(RenderFlag::ADDITIVE);
			loc = slot = 0;
			setUniforms(lpass_dir, loc, slot, scene.cam, scene.dl[i], 
											  gbuff.targets[0], gbuff.targets[3]);
			
			s0_draw(lbuff, lpass_dir, quad);
		}


		///////////////////////////////////
		// CPASS
		setFlags(RenderFlag::NONE);
		clearFramebuffer(screen, true, true);
		loc = slot = 0;
		setUniforms(cpass, loc, slot, sbuff.depthTarget);
		s0_draw(screen, cpass, quad);
	}

	context.term();
}