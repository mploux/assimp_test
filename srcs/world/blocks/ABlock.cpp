//
// Created by Marc on 11/04/2018.
//

#include <iostream>
#include "ABlock.hpp"

ABlock::ABlock(const unsigned char &texture)
	: ABlock(texture, texture, texture, texture, texture, texture)
{}

ABlock::ABlock(const unsigned char &top, const unsigned char &bottom,
			   const unsigned char &left, const unsigned char &right,
			   const unsigned char &front, const unsigned char &back)
{
	m_texture[0] = top;
	m_texture[1] = bottom;
	m_texture[2] = left;
	m_texture[3] = right;
	m_texture[4] = front;
	m_texture[5] = back;

	m_type = -1;
	m_solid = false;
	m_opaque = false;
}

ABlock::~ABlock()
{}

char ABlock::getType() const
{
	return m_type;
}

bool ABlock::isSolid() const
{
	return m_solid;
}

bool ABlock::isOpaque() const
{
	return m_opaque;
}

bool ABlock::isRendered() const
{
	if (m_type == -1)
		return false;
	return m_opaque;
}

const unsigned char *ABlock::getTextureData() const
{
	return m_texture;
}
