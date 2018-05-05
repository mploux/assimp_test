//
// Created by Marc on 11/04/2018.
//

#pragma once

#include "graphics/IRendereable.hpp"
#include "world/chunks/Chunk.hpp"

class Chunk;

class World : public IRenderable
{
private:
	Chunk	*m_chunks;
	int		m_size;
	int		m_height;

public:
	World(int size);
	virtual ~World();

	void update();
	void render(const Shader &shader);

	int getChunkIndex(const int &x, const int &y, const int &z);

	const ABlock &getBlock(const int &x, const int &y, const int &z);
};

