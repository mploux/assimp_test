//
// Created by Marc on 13/04/2018.
//

#include <regex>
#include <iostream>
#include <Core.hpp>
#include "Texture.hpp"

Texture::Texture(const std::string &path)
{
	try
	{
		if (std::regex_match(path, std::regex(".*\\.dds")))
			createDdsTexture(path);
		else
			throw std::runtime_error("Unable to load texture: " + path);

	}
	catch (std::exception &e)
	{
		std::cout << e.what() << "\n";
		Core::getInstance().terminate();
	}
}

Texture::~Texture()
{}

void Texture::createDdsTexture(const std::string &path)
{
	unsigned int blockSize;
	unsigned int offset;

	offset = 0;
	m_image = loadDds(path);
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	blockSize = (m_image.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;

	for (unsigned int level = 0; level < m_image.mipMapCount && (m_image.width || m_image.height); ++level)
	{
		unsigned int size = ((m_image.width+3)/4)*((m_image.height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, m_image.format, m_image.width, m_image.height, 0, size, m_image.data + offset);

		offset += size;

		m_image.width  /= 2;
		m_image.height /= 2;
		if(m_image.width < 1)
			m_image.width = 1;
		if(m_image.height < 1)
			m_image.height = 1;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
	delete[] m_image.data;
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}