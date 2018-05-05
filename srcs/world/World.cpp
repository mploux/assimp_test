//
// Created by Marc on 11/04/2018.
//

#include "World.hpp"

World::World(int size)
	: m_size(size), m_height(1)
{
	m_chunks = new Chunk[m_size * m_height * m_size];

	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_height; j++)
			for (int k = 0; k < m_size; k++)
				m_chunks[getChunkIndex(i, j, k)].init(this, Vec3<int>(i * CHUNK_SIZE, j * CHUNK_SIZE, k * CHUNK_SIZE));

	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_height; j++)
			for (int k = 0; k < m_size; k++)
				m_chunks[getChunkIndex(i, j, k)].generateBlocks();

	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_height; j++)
			for (int k = 0; k < m_size; k++)
				m_chunks[getChunkIndex(i, j, k)].generateRenderData();
}

World::~World()
{
	delete[] m_chunks;
}

void World::update()
{

}

void World::render(const Shader &shader)
{
	for (int i = 0; i < m_size * m_size; i++)
		m_chunks[getChunkIndex(i % m_size, 0, i / m_size)].render(shader);
}

const ABlock &World::getBlock(const int &x, const int &y, const int &z)
{
	int cx = x / CHUNK_SIZE;
	int cy = y / CHUNK_SIZE;
	int cz = z / CHUNK_SIZE;
	int bx = x % CHUNK_SIZE;
	int by = y % CHUNK_SIZE;
	int bz = z % CHUNK_SIZE;

//	if (cx < 0 || cx >= m_size || cy < 0 || cy >= m_height || cz < 0 || cz >= m_size)
//		return *GET_BLOCK("void");
	return m_chunks[getChunkIndex(cx, cy, cz)].getBlock(bx, by, bz);
}

int World::getChunkIndex(const int &x, const int &y, const int &z)
{
	int index = x * m_size * m_height + y * m_height + z;
	return index;
}
