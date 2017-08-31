
#version 450

in vec2 vUV;

layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
layout(location = 3) uniform sampler2D normalMap;
layout(location = 4) uniform vec3 L;

out vec4 outColor;
in mat4 vTBN;
in vec3 vNormal;
in vec3 vPos;
in vec3 vTangent;
in vec3 vBitangent;

mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv );

void main()
{
	vec3 tN = normalize(2*texture(normalMap, vUV).xyz-1);

	mat3 TBN = cotangent_frame(vNormal, vPos, vUV);

	vec3 N = (vTBN*vec4(tN,0)).xyz;

	

	float lamb = dot(N,-L);

	outColor =  vec4(N,1);
}



mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
{
	/* get edge vectors of the pixel triangle */
	vec3 dp1 = dFdx( p );
	vec3 dp2 = dFdy( p );
	vec2 duv1 = dFdx( uv );
	vec2 duv2 = dFdy( uv );

	/* solve the linear system */
	vec3 dp2perp = cross( dp2, N );
	vec3 dp1perp = cross( N, dp1 );
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	/* construct a scale-invariant frame */
	float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
	return mat3( T * invmax, B * invmax, N );
}