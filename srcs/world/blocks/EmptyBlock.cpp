//
// Created by Marc on 11/04/2018.
//

#include "EmptyBlock.hpp"

EmptyBlock::EmptyBlock()
	: ABlock(0, 0, 0, 0, 0, 0)
{
	m_type = 0;
	m_solid = false;
	m_opaque = false;
}

EmptyBlock::~EmptyBlock()
{

}
