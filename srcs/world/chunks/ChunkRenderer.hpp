//
// Created by Marc on 15/04/2018.
//

#pragma once

#include "graphics/IRendereable.hpp"

#define CHUNK_SIZE 16

class Chunk;

class ChunkRenderer : public IRenderable
{
private:
	static float		m_blockVertices[8 * 6 * 6];
	const Chunk			&m_chunk;
	GLuint				m_vao, m_vbo, m_tbo, m_nbo;
	int					m_renderSize;

public:
	ChunkRenderer(const Chunk &chunk);
	virtual ~ChunkRenderer();

	void generateRenderData();
	void render(const Shader &shader);

	int getRenderSize() const;
	void setRenderSize(int renderSize);
};