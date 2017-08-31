
#version 450



layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
layout(location = 3) uniform sampler2D normalMap;
layout(location = 4) uniform vec3 L;

out vec4 outColor;
in mat4 vTBN;
in vec3 vPos;
in vec2 vUV;

void main()
{
	vec4 tN = 2*texture(normalMap, vUV)-1;

	vec3 N = (vTBN*tN).xyz;

	float lamb = dot(N, -L);

	outColor =  vec4(lamb);
}