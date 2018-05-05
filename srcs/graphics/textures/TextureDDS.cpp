//
// Created by Marc on 13/04/2018.
//

#include <iostream>
#include <GL/glew.h>
#include <cstdio>
#include <cstring>
#include <Core.hpp>

#include "Texture.hpp"

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

static FILE *loadDdsFile(const char *path)
{
	FILE *result;

	result = fopen(path, "rb");
	if (result == NULL)
	{
		std::cout << "ERROR: Unable to load texture: " << path << std::endl;
		Core::getInstance().terminate();
	}
	char filecode[4];
	fread(filecode, 1, 4, result);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(result);
		return 0;
	}
	return (result);
}

static int getDdsFormat(int four_cc)
{
	if (four_cc == FOURCC_DXT1)
		return (GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
	if (four_cc == FOURCC_DXT3)
		return (GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
	if (four_cc == FOURCC_DXT5)
		return (GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
	return (0);
}

Texture::GenericImage Texture::loadDds(const std::string &path)
{
	Texture::GenericImage result;
	unsigned int linearSize;
	unsigned char header[124];
	FILE *fp;

	std::cout << "Loading: " << path << "\n";
	fp = loadDdsFile(path.c_str());
	fread(&header, 124, 1, fp);
	result.height = *(unsigned int*)&(header[8 ]);
	result.width = *(unsigned int*)&(header[12]);
	linearSize = *(unsigned int*)&(header[16]);
	result.mipMapCount = *(unsigned int*)&(header[24]);
	result.data_size = result.mipMapCount > 1 ? linearSize * 2 : linearSize;
	result.data = new unsigned char[result.data_size];
	result.format = getDdsFormat(*(unsigned int*)&(header[80]));
	fread(result.data, 1, result.data_size, fp);
	fclose(fp);

	return (result);
}