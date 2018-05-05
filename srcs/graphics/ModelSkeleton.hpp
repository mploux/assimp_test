//
// Created by mploux on 04/05/18.
//

#pragma once

#include <assimp/scene.h>
#include <GL/glew.h>
#include <maths/Mat4.hpp>
#include <map>
#include "Model.hpp"
#include "Shader.hpp"

class Model;

class ModelSkeleton
{
public:
	struct Node
	{
		const char *name;
		Mat4<float>	localMatrix;
		Mat4<float>	absoluteMatrix;
		Mat4<float>	animatedMatrix;
		Mat4<float>	animatedAbosluteMatrix;
		Node *parent;
	};

private:
	Shader								m_shader;
	const Model							&m_model;
	std::map<std::string, Node*>		m_nodes;

	GLuint					m_vao, m_vbo;
	std::vector<GLfloat>	m_boneMesh;
	int						m_boneSize;

public:
	explicit ModelSkeleton(const Model &model);
	~ModelSkeleton();

	void loadAnimNodes(const aiScene *scene);
	void addNode(Node *node);
	void buildMesh();

	void clearNodes();
	Node *getNode(const std::string &name);

	void render(Shader &shader);
};
