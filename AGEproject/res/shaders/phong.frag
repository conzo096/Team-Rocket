#version 440

struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};

struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};

uniform directional_light light;
uniform material mat;
uniform vec3 eye_pos;
uniform sampler2D tex;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

layout (location = 0) out vec4 colour;

void main()
{
	vec4 ambient = light.ambient_intensity*mat.diffuse_reflection;
	vec4 diffuse = max(dot(normal, light.light_dir), 0) * (mat.diffuse_reflection * light.light_colour);
	vec3 view_dir = (eye_pos - position) / (length(eye_pos - position));
	vec3 half_vec = (light.light_dir + view_dir) / (length(light.light_dir + view_dir));
	vec4 specular = pow((max(dot(normal, half_vec), 0)), mat.shininess) * (mat.specular_reflection * light.light_colour);
	vec4 tex_colour = texture(tex, tex_coord);
	vec4 primary = mat.emissive + ambient + diffuse;

	colour = primary * tex_colour + specular;
}