#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in lowp int in_faces;

layout (location = 2) in lowp ivec2 in_tex_0;
layout (location = 3) in lowp ivec2 in_tex_1;
layout (location = 4) in lowp ivec2 in_tex_2;

uniform mat4 projectionMatrix;

out VertexData
{
    vec4 position;
    lowp int texture[6];
    lowp int faces;

} data;

void main()
{
	data.position = vec4(in_position, 1.0);

	data.texture[0] = in_tex_0.x;
	data.texture[1] = in_tex_0.y;
	data.texture[2] = in_tex_1.x;
	data.texture[3] = in_tex_1.y;
	data.texture[4] = in_tex_2.x;
	data.texture[5] = in_tex_2.y;

	data.faces = in_faces;

	gl_Position = projectionMatrix * vec4(in_position, 1.0);
}