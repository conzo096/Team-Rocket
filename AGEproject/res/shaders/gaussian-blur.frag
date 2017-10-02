#version 440

uniform sampler2D tex;
uniform bool horizontal;

uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

layout(location = 0) in vec2 tex_coord;

layout(location = 0) out vec4 colour;

void main()
{
	vec2 tex_offset = 1.0 / textureSize(tex, 0);
	vec3 result = texture(tex, tex_coord).rgb * weight[0];
	if (horizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(tex, tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(tex, tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(tex, tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			result += texture(tex, tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		}
	}
	colour = vec4(result, 1.0);
}