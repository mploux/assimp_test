#version 400 core

#define MAX_BONES 200

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texture;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in ivec4 in_boneIDs;
layout (location = 4) in vec4 in_weights;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 bonesTransforms[MAX_BONES];

out vec4 v_position;
out vec2 v_texture;
out vec3 v_normal;
out vec3 frag_pos;
out vec4 v_weight;

void main()
{
    mat4 boneTransform =  bonesTransforms[in_boneIDs[0]] * in_weights[0];
         boneTransform += bonesTransforms[in_boneIDs[1]] * in_weights[1];
         boneTransform += bonesTransforms[in_boneIDs[2]] * in_weights[2];
         boneTransform += bonesTransforms[in_boneIDs[3]] * in_weights[3];

//    boneTransform = bonesTransforms[0];

	v_position = vec4(in_position, 1.0);
	v_texture = in_texture;
	v_normal = (modelMatrix * boneTransform * vec4(in_normal, 0.0)).xyz;

	v_weight = in_weights;

	frag_pos = vec4(in_position, 1.0).xyz;
	gl_Position = projectionMatrix * modelMatrix * boneTransform * vec4(in_position, 1.0);
}