//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "ChunkRendererG24.hpp"
#include "ChunkRendererG4.hpp"
#include "graphics/IRendereable.hpp"
#include "world/blocks/ABlock.hpp"
#include "world/World.hpp"
#include "ChunkRenderer.hpp"

#define CHUNK_SIZE 16

class World;

class Chunk : public IRenderable
{
private:
	Vec3<int>			m_pos;
	World				*m_world;
	ABlock				*m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	int					m_renderSize;

	ChunkRenderer		m_renderer;
	ChunkRendererG4		m_rendererG4;
	ChunkRendererG24	m_rendererG24;

public:
	Chunk();
	virtual ~Chunk();

	void init(World *world, const Vec3<int> &pos);

	void generateBlocks();

	void generateRenderData();
	void render(const Shader &shader);

	unsigned char getBlockVisibleFaces(const int &x, const int &y, const int &z) const;
	const ABlock &getBlock(const int &x, const int &y, const int &z) const;

	const Vec3<int> &getPosition() const;
};
