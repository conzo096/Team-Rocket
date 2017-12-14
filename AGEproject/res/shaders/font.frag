#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform vec4 texColour;

void main()
{
	color = vec4(vec3(texture( myTextureSampler, UV ) * texColour),texture( myTextureSampler, UV ).a);
	//color = texColour;
}