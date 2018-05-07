#version 400 core

out vec4 out_color;

uniform vec3 cameraPosition;
uniform sampler2D tex;

in vec2 v_texture;
in vec3 v_normal;
in vec3 frag_pos;
in vec4 v_weight;

void main()
{
	vec3 lightDirection = normalize(-frag_pos - cameraPosition);
	float lightIntensity = dot(lightDirection, v_normal) * 0.5 + 0.5;

	vec4 texture_color = texture(tex, v_texture);

	out_color = vec4(v_weight.xyz * lightIntensity, 1.0);
}
