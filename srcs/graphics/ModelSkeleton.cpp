//
// Created by mploux on 04/05/18.
//

#include <iostream>
#include <Core.hpp>
#include "ModelSkeleton.hpp"

ModelSkeleton::ModelSkeleton(const Model &model)
	: m_model(model),
	  m_shader(Shader("../data/shaders/main.1.vert", "../data/shaders/main.1.frag"))
{}

ModelSkeleton::~ModelSkeleton()
{

}

void ModelSkeleton::addNode(Node *node)
{
	m_nodes[node->name] = node;

	if (node->parent != nullptr)
	{
		m_boneMesh.push_back(node->parent->animatedAbosluteMatrix[0][3]);
		m_boneMesh.push_back(node->parent->animatedAbosluteMatrix[1][3]);
		m_boneMesh.push_back(node->parent->animatedAbosluteMatrix[2][3]);
		m_boneMesh.push_back(0.0);

		m_boneMesh.push_back(node->animatedAbosluteMatrix[0][3]);
		m_boneMesh.push_back(node->animatedAbosluteMatrix[1][3]);
		m_boneMesh.push_back(node->animatedAbosluteMatrix[2][3]);
		m_boneMesh.push_back(1.0);

		m_boneSize += 2;
	}
}

void ModelSkeleton::clearNodes()
{
	m_boneMesh.clear();
	m_boneSize = 0;
}

ModelSkeleton::Node *ModelSkeleton::getNode(const std::string &name)
{
	if (m_nodes.find(name) != m_nodes.end())
		return m_nodes[name];
	return nullptr;
}

void ModelSkeleton::buildMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_boneMesh.size(), &m_boneMesh[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * 4, (GLvoid *) 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * 4, (GLvoid *) 12);

	glBindVertexArray(0);
}

void ModelSkeleton::render(Shader &shader)
{
	m_shader.bind();
	m_shader.setUniform("projectionMatrix", Core::getInstance().getCamera().getTransformation());
	m_shader.setUniform("modelMatrix", Mat4<float>().identity());

	glPointSize(10);
	glLineWidth(3.0f);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_boneSize);
	glDrawArrays(GL_LINES, 0, m_boneSize);
	glBindVertexArray(0);

}
