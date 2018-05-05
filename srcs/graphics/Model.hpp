//
// Created by mploux on 04/05/18.
//

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <maths/Vec3.hpp>
#include <maths/Vec2.hpp>
#include <vector>
#include <GL/glew.h>
#include <maths/Mat4.hpp>
#include "ModelSkeleton.hpp"
#include "Shader.hpp"
#include "ModelAnimation.hpp"

class ModelSkeleton;

class Model
{
private:
	const aiScene		*m_scene;

	std::vector<Vec3<float> >			m_positions;
	std::vector<Vec2<float> >			m_uvs;
	std::vector<Vec3<float> >			m_normals;

	GLsizei				m_drawSize;
	GLfloat				*m_vBuffer;
	GLfloat				*m_uBuffer;
	GLfloat				*m_nBuffer;

	GLuint				m_vao, m_vbo, m_ubo, m_nbo, m_bbo;

	ModelSkeleton		*m_skeleton;
	ModelAnimation		*m_animations;

	void loadNode(float animationTime, aiNode *node, aiMatrix4x4 &parent, ModelSkeleton::Node *parentNode);

public:
	Model(const std::string &file);
	~Model();

	void addVertex(Vec3<float> v);
	void addUV(Vec2<float> v);
	void addNormal(Vec3<float> v);

	void build();

	void render(Shader &shader);
};
