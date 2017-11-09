#version 440
out vec4 outColour;
// Texture.
uniform sampler2D tex;
layout (location = 1) in vec2 texCoords;


void main()
{
	outColour = glm::vec4(1,0,0,1);
	//outColour = texture(tex,texCoords);
}