//
// Created by mploux on 14/04/18.
//

#include "ChunkRendererG24.hpp"
#include "Chunk.hpp"

ChunkRendererG24::ChunkRendererG24(const Chunk &chunk)
	: m_chunk(chunk)
{}

ChunkRendererG24::~ChunkRendererG24()
{}

void ChunkRendererG24::generateRenderData()
{
	GLfloat vertexData[m_renderSize * 3];
	GLubyte textureData[m_renderSize * 6];
	GLubyte facesData[m_renderSize];

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
					vertexData[vertexSize++] = m_chunk.getPosition().getX() + static_cast<float>(x);
					vertexData[vertexSize++] = m_chunk.getPosition().getY() + static_cast<float>(y);
					vertexData[vertexSize++] = m_chunk.getPosition().getZ() + static_cast<float>(z);

					textureData[textureSize++] = block.getTextureData()[0];
					textureData[textureSize++] = block.getTextureData()[1];
					textureData[textureSize++] = block.getTextureData()[2];
					textureData[textureSize++] = block.getTextureData()[3];
					textureData[textureSize++] = block.getTextureData()[4];
					textureData[textureSize++] = block.getTextureData()[5];

					facesData[facesSize++] = visibleFaces;

					m_renderSize++;
				}
			}
		}
	}

	generateVertexBuffer(
			sizeof(GLfloat) * vertexSize, vertexData,
			sizeof(GLubyte) * textureSize, textureData,
			sizeof(GLubyte) * facesSize, facesData);
}

void ChunkRendererG24::generateVertexBuffer(const int &dataSize, GLfloat *data, const int &textureSize, GLubyte *texture, const int &facesSize, GLubyte *faces)
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
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, textureSize, texture, GL_STATIC_DRAW);
	glVertexAttribIPointer(2, 2, GL_UNSIGNED_BYTE, 6 * 1, (void *) 0);
	glVertexAttribIPointer(3, 2, GL_UNSIGNED_BYTE, 6 * 1, (void *) 2);
	glVertexAttribIPointer(4, 2, GL_UNSIGNED_BYTE, 6 * 1, (void *) 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ChunkRendererG24::render(const Shader &shader)
{
	(void) shader;

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_renderSize);
	glBindVertexArray(0);
}

int ChunkRendererG24::getRenderSize() const
{
	return m_renderSize;
}

void ChunkRendererG24::setRenderSize(int renderSize)
{
	m_renderSize = renderSize;
}
