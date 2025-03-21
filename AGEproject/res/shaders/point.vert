#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinate
layout (location = 10) in vec2 tex_coord_in;

// Outgoing vertex position
layout (location = 0) out vec3 vertex_position;
// Outgoing transformed normal
layout (location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;

void main()
{
	// Set position
	gl_Position = MVP * vec4(position, 1);
	// **************************************
	// Output other values to fragment shader
	// **************************************
	vec4 vertex_position4 = M * vec4(position, 1);
	vertex_position.x = vertex_position4.x;
	vertex_position.y = vertex_position4.y;
	vertex_position.z = vertex_position4.z;
	transformed_normal = N * normal;
	tex_coord_out = tex_coord_in;

}