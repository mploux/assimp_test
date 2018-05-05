#version 400 core

#define TOP_SHADING 1
#define X_SHADING 0.8
#define Z_SHADING 0.6
#define BOTTOM_SHADING 0.4

#define TEXTURE_SIZE 16

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

uniform mat4 projectionMatrix;

in VertexData
{
    vec4 position;
    lowp int texture[6];
    lowp int faces;

} data[];

out vec4 v_color;
out vec3 v_normal;
out vec2 v_texcoord;
out vec4 frag_position;

void addVertex(vec2 texcoord, vec3 offset, float tx, float ty)
{
	v_texcoord = vec2((tx + texcoord.x) / TEXTURE_SIZE , (ty + texcoord.y) / TEXTURE_SIZE);
	frag_position = (data[0].position + vec4(offset.xyz, 0.0));
    gl_Position = projectionMatrix * frag_position;
    EmitVertex();
}

void draw_top()
{
	v_color = vec4(TOP_SHADING, TOP_SHADING, TOP_SHADING, 1);
	v_normal = vec3(0, 1, 0);

	float tx = data[0].texture[0] % TEXTURE_SIZE;
	float ty = data[0].texture[0] / TEXTURE_SIZE;

	addVertex(vec2(0.0, 0.0), vec3(0.0, 1.0, 1.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(1.0, 1.0, 1.0), tx, ty);
	addVertex(vec2(0.0, 1.0), vec3(0.0, 1.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 1.0), vec3(1.0, 1.0, 0.0), tx, ty);

    EndPrimitive();
}

void draw_bottom()
{
	v_color = vec4(BOTTOM_SHADING, BOTTOM_SHADING, BOTTOM_SHADING, 1);
	v_normal = vec3(0, -1, 0);

	float tx = data[0].texture[1] % TEXTURE_SIZE;
	float ty = data[0].texture[1] / TEXTURE_SIZE;

	addVertex(vec2(0.0, 0.0), vec3(0.0, 0.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(1.0, 0.0, 0.0), tx, ty);
	addVertex(vec2(0.0, 1.0), vec3(0.0, 0.0, 1.0), tx, ty);
	addVertex(vec2(1.0, 1.0), vec3(1.0, 0.0, 1.0), tx, ty);

    EndPrimitive();
}

void draw_left()
{
	v_color = vec4(X_SHADING, X_SHADING, X_SHADING, 1);
	v_normal = vec3(-1, 0, 0);

	float tx = data[0].texture[2] % TEXTURE_SIZE;
	float ty = data[0].texture[2] / TEXTURE_SIZE;

	addVertex(vec2(0.0, 1.0), vec3(0.0, 0.0, 1.0), tx, ty);
	addVertex(vec2(0.0, 0.0), vec3(0.0, 1.0, 1.0), tx, ty);
	addVertex(vec2(1.0, 1.0), vec3(0.0, 0.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(0.0, 1.0, 0.0), tx, ty);

    EndPrimitive();
}

void draw_right()
{
	v_color = vec4(X_SHADING, X_SHADING, X_SHADING, 1);
	v_normal = vec3(1, 0, 0);

	float tx = data[0].texture[3] % TEXTURE_SIZE;
	float ty = data[0].texture[3] / TEXTURE_SIZE;

	addVertex(vec2(0.0, 1.0), vec3(1.0, 0.0, 0.0), tx, ty);
	addVertex(vec2(0.0, 0.0), vec3(1.0, 1.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 1.0), vec3(1.0, 0.0, 1.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(1.0, 1.0, 1.0), tx, ty);

    EndPrimitive();
}

void draw_front()
{
	v_color = vec4(Z_SHADING, Z_SHADING, Z_SHADING, 1);
	v_normal = vec3(0, 0, -1);

	float tx = data[0].texture[4] % TEXTURE_SIZE;
	float ty = data[0].texture[4] / TEXTURE_SIZE;

	addVertex(vec2(0.0, 0.0), vec3(0.0, 1.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(1.0, 1.0, 0.0), tx, ty);
	addVertex(vec2(0.0, 1.0), vec3(0.0, 0.0, 0.0), tx, ty);
	addVertex(vec2(1.0, 1.0), vec3(1.0, 0.0, 0.0), tx, ty);

    EndPrimitive();
}

void draw_back()
{
	v_color = vec4(Z_SHADING, Z_SHADING, Z_SHADING, 1);
	v_normal = vec3(0, 0, 1);

	float tx = data[0].texture[5] % TEXTURE_SIZE;
	float ty = data[0].texture[5] / TEXTURE_SIZE;

	addVertex(vec2(1.0, 1.0), vec3(0.0, 0.0, 1.0), tx, ty);
	addVertex(vec2(0.0, 1.0), vec3(1.0, 0.0, 1.0), tx, ty);
	addVertex(vec2(1.0, 0.0), vec3(0.0, 1.0, 1.0), tx, ty);
	addVertex(vec2(0.0, 0.0), vec3(1.0, 1.0, 1.0), tx, ty);

    EndPrimitive();
}

void main()
{
	if ((data[0].faces & (1 << 0)) != 0) draw_top();
	if ((data[0].faces & (1 << 1)) != 0) draw_bottom();
	if ((data[0].faces & (1 << 2)) != 0) draw_left();
	if ((data[0].faces & (1 << 3)) != 0) draw_right();
	if ((data[0].faces & (1 << 4)) != 0) draw_front();
	if ((data[0].faces & (1 << 5)) != 0) draw_back();
}
