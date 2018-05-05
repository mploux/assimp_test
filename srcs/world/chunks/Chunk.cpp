//
// Created by Marc on 11/04/2018.
//

#include <iostream>
#include <cstdlib>
#include <Core.hpp>
#include "Chunk.hpp"

Chunk::Chunk()
	: m_pos(ZERO), m_world(nullptr), m_renderer(ChunkRenderer(*this)), m_rendererG4(ChunkRendererG4(*this)), m_rendererG24(ChunkRendererG24(*this))
{
}

Chunk::~Chunk()
{

}

void Chunk::init(World *world, const Vec3<int> &pos)
{
	m_pos = pos;
	m_world = world;
}

void Chunk::generateBlocks()
{
//	m_renderSize = 0;
//	std::srand(0);
//	for (int x = 0; x < CHUNK_SIZE; x++)
//	{
//		for (int y = 0; y < CHUNK_SIZE; y++)
//		{
//			for (int z = 0; z < CHUNK_SIZE; z++)
//			{
//				if (y < 10 + std::rand() % 4)
//				{
//					int rnd = std::rand() % 2;
//					if (rnd == 0)
//						m_blocks[x][y][z] = GET_BLOCK("stone");
//					else if (rnd == 1)
//						m_blocks[x][y][z] = GET_BLOCK("tnt");
//
//					m_renderSize++;
//
//				}
//				else
//					m_blocks[x][y][z] = GET_BLOCK("empty");
//			}
//		}
//	}
//
//	m_renderer.setRenderSize(m_renderSize);
//	m_rendererG4.setRenderSize(m_renderSize);
//	m_rendererG24.setRenderSize(m_renderSize);
}

void Chunk::generateRenderData()
{
	m_renderer.generateRenderData();
	m_rendererG4.generateRenderData();
	m_rendererG24.generateRenderData();
}

void Chunk::render(const Shader &shader)
{
//	if (Core::getInstance().getRenderMode() == RENDER_G_4)
//		m_rendererG4.render(shader);
//	if (Core::getInstance().getRenderMode() == RENDER_G_24)
//		m_rendererG24.render(shader);
//	if (Core::getInstance().getRenderMode() == RENDER_VAO)
//		m_renderer.render(shader);
}

unsigned char Chunk::getBlockVisibleFaces(const int &x, const int &y, const int &z) const
{
	unsigned char result = 0;

	bool top = m_world->getBlock(m_pos.getX() + x, m_pos.getY() + y + 1, m_pos.getZ() + z).isRendered();
	bool bottom = m_world->getBlock(m_pos.getX() + x, m_pos.getY() + y - 1, m_pos.getZ() + z).isRendered();
	bool left = m_world->getBlock(m_pos.getX() + x - 1, m_pos.getY() + y, m_pos.getZ() + z).isRendered();
	bool right = m_world->getBlock(m_pos.getX() + x + 1, m_pos.getY() + y, m_pos.getZ() + z).isRendered();
	bool front = m_world->getBlock(m_pos.getX() + x, m_pos.getY() + y, m_pos.getZ() + z - 1).isRendered();
	bool back = m_world->getBlock(m_pos.getX() + x, m_pos.getY() + y, m_pos.getZ() + z + 1).isRendered();

	result |= top 		? 0 : (1 << 0);
	result |= bottom	? 0 : (1 << 1);
	result |= left		? 0 : (1 << 2);
	result |= right		? 0 : (1 << 3);
	result |= front		? 0 : (1 << 4);
	result |= back		? 0 : (1 << 5);

	return result;
}

const ABlock &Chunk::getBlock(const int &x, const int &y, const int &z) const
{
//	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
//		return *GET_BLOCK("void");
	return *m_blocks[x][y][z];
}

const Vec3<int> &Chunk::getPosition() const {
	return m_pos;
}