//
// Created by Marc on 11/04/2018.
//

#include "StoneBlock.hpp"

StoneBlock::StoneBlock()
	: ABlock(1)
{
	m_type = 1;
	m_solid = true;
	m_opaque = true;
}

StoneBlock::~StoneBlock()
{
}
