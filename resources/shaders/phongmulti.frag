
#version 450

// Camera
layout(location = 1) uniform mat4 view;

// surface maps
layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 4) uniform sampler2D normalMap;
layout(location = 5) uniform sampler2D specularMap;
layout(location = 6) uniform float gloss;

// light data
layout(location =  7) uniform vec3  lightDir;
layout(location =  8) uniform vec4  lightColor;
layout(location =  9) uniform float lightIntensity;
layout(location = 10) uniform vec4  lightAmbient;

// Vertex inputs
in mat4 vTBN;
in vec3 vPos;
in vec2 vUV;

// Framebuffer outputs
layout(location = 0) out vec4 outFinal;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outSpecular;
layout(location = 3) out vec4 outNormal;

float calc_lambert(vec3 N, vec3 L);

float calc_phong(vec3 N, vec3 L, vec3 P, vec3 C, float power);

float calc_ambient(vec3 N, vec3 P);

void main()
{
	// read surface data from maps
	vec3 normal   = (vTBN*(2*texture(normalMap, vUV)-1)).xyz;
	vec4 diffuse  = texture(diffuseMap, vUV);
	vec4 specular = texture(specularMap, vUV);

	// calculate lighting factors
	float ambi = calc_ambient(normal, vPos);
	float lamb = calc_lambert(normal, lightDir);
	float spec = 0;
	if(lamb > 0)
		spec = calc_phong(normal, lightDir, vPos, view[3].xyz, gloss);

	// for debugging
	outNormal = vec4(normal, 1);

	outNormal = vec4(1,1,0,1);

	// calculate color terms
	vec4 ambient = diffuse  * ambi * lightAmbient;
	outDiffuse   = diffuse  * lamb * lightColor * lightIntensity;
	outSpecular  = specular * spec * lightColor * lightIntensity;

	// aggregate for final color
	outFinal  = ambient + outDiffuse + outSpecular;
}


float calc_lambert(vec3 N, vec3 L)
{
	return max(0, dot(N,-L));
}

float calc_phong(vec3 N, vec3 L, vec3 P, vec3 C, float power)
{
	vec3 R = reflect(L, N);
	vec3 E = normalize(C - P);
	return pow(max(0,dot(E,-R)), power);
}

float calc_ambient(vec3 N, vec3 P)
{
	return max(0, dot(N,normalize(P)));
}