//
// Created by mploux on 14/04/18.
//

#include "ChunkRendererG4.hpp"
#include "Chunk.hpp"

ChunkRendererG4::ChunkRendererG4(const Chunk &chunk)
	: m_chunk(chunk)
{}

ChunkRendererG4::~ChunkRendererG4()
{}

void ChunkRendererG4::generateRenderData()
{
	GLfloat vertexData[m_renderSize * 6 * 3];
	GLubyte textureData[m_renderSize * 6];
	GLubyte facesData[m_renderSize * 6];

	m_renderSize = 0;
	int vertexSize = 0;
	int textureSize = 0;
	int facesSize = 0;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				ABlock block = m_chunk.getBlock(x, y, z);
				unsigned char visibleFaces = m_chunk.getBlockVisibleFaces(x, y, z);
				if (block.getType() != 0 && visibleFaces != 0)
				{
					for (int i = 0; i < 6; i++)
					{
						if (visibleFaces & (1 << i))
						{
							vertexData[vertexSize++] = m_chunk.getPosition().getX() + static_cast<float>(x);
							vertexData[vertexSize++] = m_chunk.getPosition().getY() + static_cast<float>(y);
							vertexData[vertexSize++] = m_chunk.getPosition().getZ() + static_cast<float>(z);

							textureData[textureSize++] = block.getTextureData()[i];

							facesData[facesSize++] = static_cast<GLubyte >(i);

							m_renderSize++;
						}
					}
				}
			}
		}
	}

	generateVertexBuffer(
			sizeof(GLfloat) * vertexSize, vertexData,
			sizeof(GLubyte) * textureSize, textureData,
			sizeof(GLubyte) * facesSize, facesData);
}

void ChunkRendererG4::generateVertexBuffer(const int &dataSize, GLfloat *data, const int &textureSize, GLubyte *texture, const int &facesSize, GLubyte *faces)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_tbo);
	glGenBuffers(1, &m_dbo);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_dbo);
	glBufferData(GL_ARRAY_BUFFER, facesSize, faces, GL_STATIC_DRAW);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, 0, nullptr);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, textureSize, texture, GL_STATIC_DRAW);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ChunkRendererG4::render(const Shader &shader)
{
	(void) shader;

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_renderSize);
	glBindVertexArray(0);
}

int ChunkRendererG4::getRenderSize() const
{
	return m_renderSize;
}

void ChunkRendererG4::setRenderSize(int renderSize)
{
	m_renderSize = renderSize;
}
