
#version 450


layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform sampler2D shadowmap;

layout(location = 6) uniform mat4 lproj1;
layout(location = 7) uniform mat4 lview1;
layout(location = 8) uniform sampler2D shadowmap1;


in vec2 vUV;
in vec4 vPos;

uniform float shadowBias = 0.01;

// Simple matrix that converts from clip space (-1,1) to UV space (0,1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

out vec4 outColor;

float pcf(in sampler2D shadowMap, in vec4 shadowPosition, int iterations);

void main()
{
	// -1 to 1 range is in clip space, but the shadow buffer is in 0-1 UV space
	// [-1, 1] * .5 = [-.5,.5] + .5 = [0,1]
	vec4 sUV = clipToUV * lproj * lview * vPos;	

	float visibility = 1;
	//if(texture(shadowmap,sUV.xy).r  < sUV.z - shadowBias)
		visibility = pcf(shadowmap, sUV, 8);



	vec4 sUV1 = clipToUV * lproj1 * lview1 * vPos;	

	float visibility1 = 1;
	//if(texture(shadowmap,sUV.xy).r  < sUV.z - shadowBias)
		visibility1 = pcf(shadowmap1, sUV1, 8);


	outColor = vec4(1,1,0,1) * visibility + vec4(0,0,1,1) * visibility1;
}


// takes in a shadow map and a projected fragment position to determine
// how much in shadow we are based upon the number of iterations.
float pcf(in sampler2D shadowMap, in vec4 shadowPosition, int iterations)
{
	vec2 sDim = textureSize(shadowMap,0).xy;
	float retval = 0;
	
	vec2 uv = shadowPosition.xy;
	float z = shadowPosition.z - 0.01;

	//	if(texture(shadowmap,sUV.xy).r  < sUV.z - shadowBias)
	for(int i = -iterations; i <= iterations; ++i)
	{		
		if(!(texture(shadowMap, uv + vec2(i,0)/sDim).r < z))
			retval++;

		if(!(texture(shadowMap, uv + vec2(0,i)/sDim).r < z))
			retval++;
	}
	return retval / (iterations*4.0);
}