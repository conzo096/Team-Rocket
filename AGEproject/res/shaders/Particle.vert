#version 440

// view projection matrix
uniform mat4 VP;
uniform vec3 up;
uniform vec3 right;
// Position of mesh, this should be in layout location - fix later.
uniform vec3 pos;

// Temp bandAid.
uniform mat4 MVP;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

layout (location = 0) out vec2 outTexCoords;
void main()
{
	vec3 vertexPos = pos + position;
	gl_Position =  VP * vec4(vertexPos, 1.0);

	outTexCoords = texCoords;
}