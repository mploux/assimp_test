//
// Created by Marc on 15/04/2018.
//

#include <iostream>
#include "ChunkRenderer.hpp"
#include "Chunk.hpp"

float ChunkRenderer::m_blockVertices[8 * 6 * 6] =
{
	0, 1, 0,	0, 1,	0, 1, 0,
	0, 1, 1,	0, 0,	0, 1, 0,
	1, 1, 1,	1, 0,	0, 1, 0,
	0, 1, 0,	0, 1,	0, 1, 0,
	1, 1, 1,	1, 0,	0, 1, 0,
	1, 1, 0,	1, 1,	0, 1, 0,

	0, 0, 0,	0, 0,	0, -1, 0,
	1, 0, 1,	1, 1,	0, -1, 0,
	0, 0, 1,	0, 1,	0, -1, 0,
	0, 0, 0,	0, 0,	0, -1, 0,
	1, 0, 0,	1, 0,	0, -1, 0,
	1, 0, 1,	1, 1,	0, -1, 0,

	0, 0, 0,	1, 1,	-1, 0, 0,
	0, 0, 1,	0, 1,	-1, 0, 0,
	0, 1, 1,	0, 0,	-1, 0, 0,
	0, 0, 0,	1, 1,	-1, 0, 0,
	0, 1, 1,	0, 0,	-1, 0, 0,
	0, 1, 0,	1, 0,	-1, 0, 0,

	1, 0, 0,	0, 1,	1, 0, 0,
	1, 1, 1,	1, 0,	1, 0, 0,
	1, 0, 1,	1, 1,	1, 0, 0,
	1, 0, 0,	0, 1,	1, 0, 0,
	1, 1, 0,	0, 0,	1, 0, 0,
	1, 1, 1,	1, 0,	1, 0, 0,

	0, 0, 0,	0, 1,	0, 0, -1,
	0, 1, 0,	0, 0,	0, 0, -1,
	1, 1, 0,	1, 0,	0, 0, -1,
	0, 0, 0,	0, 1,	0, 0, -1,
	1, 1, 0,	1, 0,	0, 0, -1,
	1, 0, 0,	1, 1,	0, 0, -1,

	0, 0, 1,	1, 1,	0, 0, 1,
	1, 1, 1,	0, 0,	0, 0, 1,
	0, 1, 1,	1, 0,	0, 0, 1,
	0, 0, 1,	1, 1,	0, 0, 1,
	1, 0, 1,	0, 1,	0, 0, 1,
	1, 1, 1,	0, 0,	0, 0, 1
};

ChunkRenderer::ChunkRenderer(const Chunk &chunk)
	: m_chunk(chunk)
{}

ChunkRenderer::~ChunkRenderer()
{}

void ChunkRenderer::generateRenderData()
{
	GLfloat *vertexData = new GLfloat[m_renderSize * 6 * 6 * 3];
	GLfloat *textureData = new GLfloat[m_renderSize * 6 * 6 * 2];
	GLfloat *normalData = new GLfloat[m_renderSize * 6 * 6 * 3];

	m_renderSize = 0;
	int vertexSize = 0;
	int textureSize = 0;
	int normalSize = 0;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				ABlock block = m_chunk.getBlock(x, y, z);

				Vec3<float> absPos = Vec3<float>(
						m_chunk.getPosition().getX() + static_cast<float>(x),
						m_chunk.getPosition().getY() + static_cast<float>(y),
						m_chunk.getPosition().getZ() + static_cast<float>(z));

				unsigned char visibleFaces = m_chunk.getBlockVisibleFaces(x, y, z);
				if (block.getType() != 0 && visibleFaces != 0)
				{
					for (int i = 0; i < 6; i++)
					{
						int texture = block.getTextureData()[i];
						int tx = texture % 16;
						int ty = texture / 16;
						if (visibleFaces & (1 << i))
						{
							for (int j = 0; j < 6; j++)
							{
								vertexData[vertexSize++] = absPos.getX() + m_blockVertices[i * 48 + j * 8 + 0];
								vertexData[vertexSize++] = absPos.getY() + m_blockVertices[i * 48 + j * 8 + 1];
								vertexData[vertexSize++] = absPos.getZ() + m_blockVertices[i * 48 + j * 8 + 2];

								textureData[textureSize++] = (m_blockVertices[i * 48 + j * 8 + 3] + tx) / 16.0f;
								textureData[textureSize++] = (m_blockVertices[i * 48 + j * 8 + 4] + ty) / 16.0f;

								normalData[normalSize++] = m_blockVertices[i * 48 + j * 8 + 5];
								normalData[normalSize++] = m_blockVertices[i * 48 + j * 8 + 6];
								normalData[normalSize++] = m_blockVertices[i * 48 + j * 8 + 7];
							}
							m_renderSize += 6;
						}
					}
				}
			}
		}
	}

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_tbo);
	glGenBuffers(1, &m_nbo);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexSize, vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * textureSize, textureData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalSize, normalData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	delete[] vertexData;
	delete[] textureData;
	delete[] normalData;
}

void ChunkRenderer::render(const Shader &shader)
{
	(void) shader;

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_renderSize);
	glBindVertexArray(0);
}

int ChunkRenderer::getRenderSize() const
{
	return m_renderSize;
}

void ChunkRenderer::setRenderSize(int renderSize)
{
	m_renderSize = renderSize;
}
