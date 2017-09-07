
#version 450

// Vertex Data
in vec2 vUV;

// Camera Data
layout(location = 1) uniform mat4 view;

// Light Data
layout(location = 2) uniform mat4 lightProj;
layout(location = 3) uniform mat4 lightView; // lightDirection is the forward now!
layout(location = 4) uniform vec4 lightColor;
layout(location = 5) uniform float intensity;


layout(location = 6) uniform sampler2D diffuseMap;
layout(location = 7) uniform sampler2D normalMap;


// Framebuffer Outputs
layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outSpecular;


void main()
{
	vec3 L = (view * lightView[2]).xyz;
	vec3 N = (texture(normalMap, vUV)).xyz;

	float lamb = max(0, dot(-L, N));

	outDiffuse = texture(diffuseMap,vUV) * lamb * lightColor * intensity;
}