#version 440

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

layout(location = 0) in vec2 texCoords;
layout(location = 0) out vec4 colour;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColour = texture(scene, texCoords).xyz;
	vec3 bloomColour = texture(bloomBlur, texCoords).xyz;
	if (bloom)
		hdrColour += bloomColour;
	vec3 result = vec3(1.0) - exp(-hdrColour * exposure);    
	result = pow(result, vec3(1.0 / gamma));
	colour = vec4(result, 1.0f);
}