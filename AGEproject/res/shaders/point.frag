#version 440

// Point light information
struct point_light
{
	vec4 light_colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

// Material information
struct material
{
	vec4 emissive;

	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};

// Point light for the scene
uniform point_light point;
// Material for the object
uniform material mat;
// Eye position
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;
// Incoming texture coordinate
layout (location = 2) in vec2 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	// *******************************************
	// Get distance between point light and vertex
	// *******************************************
	float d = distance(point.position, position);

	// ****************************
	// Calculate attenuation factor
	// ****************************
	float fac = 1.0f;
	float attenuation = 1 / (d * d * fac + d * fac + fac);//not in use yet.

	// **********************
	// Calculate light colour
	// **********************
	//vec4 light_colour = point.light_colour * (point.constant / d);
	vec4 light_colour = point.light_colour / point.quadratic;
	//light_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f) / (point.quadratic / d);
	//light_colour = vec4(0.5f, 0.3f, 0.0f, 1.0f);// *attenuation;

	// *******************
	// Calculate light dir
	// *******************
	vec3 light_dir = normalize(point.position - position);

	// ******************************************************************************
	// Now use standard phong shading but using calculated light colour and direction
	// - note no ambient
	// ******************************************************************************
	
	float k = max(dot(normalize(normal), light_dir), 0);
	vec4 diffuse = k * (mat.diffuse_reflection * light_colour);

	// ************************
	// Calculate view direction
	// ************************
	vec4 world_position4 = vec4(position, 1.0) * mat.diffuse_reflection;
	vec3 world_position = world_position4.xyz; // Think this works

	vec3 view_dir = normalize(eye_pos - world_position);

	// *********************
	// Calculate half vector
	// *********************
	vec3 H = normalize(light_dir + view_dir);

	// ****************************
	// Calculate specular component
	// ****************************
	float k2 = pow(max(dot(normalize(normal), H), 0), mat.shininess);
	vec4 specular = k2 * (mat.specular_reflection * light_colour);

	// **************
	// Sample texture
	// **************
	vec4 tex_colour = texture(tex, tex_coord);

	// **********************************
	// Calculate primary colour component
	// **********************************
	vec4 primary = mat.emissive + diffuse;
	vec4 secondary = specular;
	secondary.w = 1.0;
	primary.w = 1.0;

	// **********************
	// Calculate final colour
	// - remember alpha 1.0
	// **********************
	colour = primary * tex_colour + secondary;

}