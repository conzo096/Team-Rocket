#version 440

uniform sampler2D tex;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main()
{
	colour = texture(tex, tex_coord);
	
	//colour = vec4(0,1,0,1);
}