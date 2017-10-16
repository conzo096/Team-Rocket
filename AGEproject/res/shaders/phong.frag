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

struct PointLight
{
	vec4 ambient, diffuse, specular;
	vec3 position;
	float range;
	float constant, linear, quadratic;
};

uniform directional_light light;
uniform material mat;
uniform vec3 eye_pos;
uniform sampler2D tex;

#define NUM_POINT_LIGHTS 16

uniform PointLight point_light[NUM_POINT_LIGHTS];
//uniform PointLight point_light[0];
//uniform PointLight obvious_name;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec4 colour;

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

vec4 calculate_point(PointLight p, material m, vec3 pos, vec3 normal, vec3 view_dir, vec4 tex_colour)
{
	float distance = length(p.position - pos);
	vec4 ambient = vec4(0, 0, 0, 1);
	vec4 diffuse = vec4(0, 0, 0, 1); 
	vec4 specular = vec4(0, 0, 0, 1);

	if (distance < p.range)
	{
		vec3 light_dir = normalize(p.position - pos);

		float diff = max(dot(normal, light_dir), 0.0);

		vec3 H = normalize(light_dir + view_dir);
		float k2 = pow(max(dot(normalize(normal), H), 0), m.shininess);

		float attenuation = 1.0 / (p.constant + p.linear * distance + p.quadratic * (distance * distance));

		ambient = p.ambient  *        m.diffuse_reflection;
		diffuse = p.diffuse  * diff * m.diffuse_reflection;
		specular = p.specular * k2 * m.specular_reflection;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}

	// Final
	vec4 primary = m.emissive + ambient + diffuse;
	vec4 result = primary * tex_colour + specular;
	result.w = 1.0;
	return result;
}

void main()
{
	vec3 view_dir = normalize(eye_pos - position);
	vec3 norm = normalize(normal);
	vec4 tex_colour = texture(tex, tex_coord);

	colour = tex_colour;

	//colour = calculate_point(obvious_name, mat, position, norm, view_dir, tex_colour);

	//	colour = obvious_name.diffuse;

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
		colour += calculate_point(point_light[i], mat, position, normal, view_dir, tex_colour);
	//	for(int i = 0; i < num_spot_lights; i++)
	//	do_directional_light(light);
}