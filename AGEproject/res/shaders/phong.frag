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

struct point_light
{
	vec4 ambient, diffuse, specular;
	vec3 position;
	float range;
};

uniform directional_light light;
uniform material mat;
uniform vec3 eye_pos;
uniform sampler2D tex;

uniform point_light points[1];

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

layout (location = 0) out vec4 colour;

/*
void main()
{
	vec4 ambient = light.ambient_intensity * mat.diffuse_reflection;
	vec4 diffuse = max(dot(normal, light.light_dir), 0) * (mat.diffuse_reflection * light.light_colour);
	vec3 view_dir = (eye_pos - position) / (length(eye_pos - position));
	vec3 half_vec = (light.light_dir + view_dir) / (length(light.light_dir + view_dir));
	vec4 specular = pow((max(dot(normal, half_vec), 0)), mat.shininess) * (mat.specular_reflection * light.light_colour);
	vec4 tex_colour = texture(tex, tex_coord);
	vec4 primary = mat.emissive + ambient + diffuse;

	colour = primary * tex_colour + specular;
}
*/

/*
vec4 do_directional_light(directional_light d)
{
	vec4 ambient = d.ambient_intensity * mat.diffuse_reflection;
	vec4 diffuse = max(dot(normal, d.light_dir), 0) * (mat.diffuse_reflection * d.light_colour);
	vec3 view_dir = (eye_pos - position) / (length(eye_pos - position));
	vec3 half_vec = (d.light_dir + view_dir) / (length(d.light_dir + view_dir));
	vec4 specular = pow((max(dot(normal, half_vec), 0)), mat.shininess) * (mat.specular_reflection * d.light_colour);
	vec4 tex_colour = texture(tex, tex_coord);
	vec4 primary = mat.emissive + ambient + diffuse;

	return (primary * tex_colour + specular);
}
*/

vec4 calculate_point(in point_light p, in material m, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
	vec4 diffuse = vec4(0, 0, 0, 1);
	vec4 specular = vec4(0, 0, 0, 1);
	vec4 ambient = vec4(0, 0, 0, 1);

	if (length(p.position - position) < p.range)
	{
		// Diffuse
		vec3 light_dir = normalize(p.position - position);
		ambient = p.ambient * m.diffuse_reflection;
		diffuse = max(dot(normal, light_dir), 0.0) * (m.diffuse_reflection * p.diffuse);

		// Specular
		vec3 half_vec = normalize(light_dir + view_dir);
		specular = pow(max(dot(normal, half_vec), 0.0), m.shininess) * (m.specular_reflection * p.specular);		
	}

	// Final
	vec4 primary = m.emissive + ambient + diffuse;
	return (primary * tex_colour + specular);
}

void main()
{
	vec3 view_dir = normalize(eye_pos - position);
	vec4 tex_colour = texture(tex, tex_coord);

	colour = tex_colour;

	colour += calculate_point(points[0], mat, position, normal, view_dir, tex_colour);
//	for(int i = 0; i < num_spot_lights; i++)
//	do_directional_light(light);
}