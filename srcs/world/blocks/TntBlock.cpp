//
// Created by Marc on 14/04/2018.
//

#include "TntBlock.hpp"

TntBlock::TntBlock()
	: ABlock(9, 10, 8, 8, 8, 8)
{
	m_type = 8;
	m_solid = true;
	m_opaque = true;
}

TntBlock::~TntBlock()
{

}
