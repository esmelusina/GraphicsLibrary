
#version 450



layout(location = 1) uniform sampler2D textureMap;

in vec2 vUV;

out vec4 outColor;

void main()
{
	outColor = texture(textureMap,vUV);

}