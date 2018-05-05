//
// Created by mploux on 14/04/18.
//

#pragma once

#include "graphics/IRendereable.hpp"

#define CHUNK_SIZE 16

class Chunk;

class ChunkRendererG4 : public IRenderable
{
private:
	const Chunk	&m_chunk;
	GLuint		m_vao, m_vbo, m_tbo, m_dbo;
	int			m_renderSize;

public:
	ChunkRendererG4(const Chunk &chunk);
	virtual ~ChunkRendererG4();

	void generateRenderData();
	void generateVertexBuffer(const int &dataSize, GLfloat *data, const int &textureSize, GLubyte *texture, const int &facesSize, GLubyte *faces);
	void render(const Shader &shader);

	int getRenderSize() const;
	void setRenderSize(int renderSize);
};