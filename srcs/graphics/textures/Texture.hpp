//
// Created by Marc on 13/04/2018.
//

#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	struct GenericImage
	{
		unsigned int	width;
		unsigned int	height;
		unsigned char	*data;
		unsigned int	data_size;
		unsigned int	mipMapCount;
		unsigned int	format;
	};

	GLuint			m_id;
	GenericImage	m_image;

	void createBmpTexture(const std::string &path);
	void createDdsTexture(const std::string &path);

	GenericImage loadDds(const std::string &path);
	GenericImage loadImage(const std::string &path);

public:
	Texture(const std::string &path);
	virtual ~Texture();

	void bind();
	void unbind();

	GenericImage getImage() { return m_image; }

};

