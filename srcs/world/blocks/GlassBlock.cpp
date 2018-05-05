//
// Created by Marc on 14/04/2018.
//

#include "GlassBlock.hpp"

GlassBlock::GlassBlock()
		: ABlock(16 * 3 + 1)
{
	m_type = 16 * 3 + 1;
	m_solid = true;
	m_opaque = false;
}

GlassBlock::~GlassBlock()
{

}