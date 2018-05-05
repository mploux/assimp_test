//
// Created by mploux on 14/04/18.
//

#include "Blocks.hpp"
#include "TntBlock.hpp"
#include "StoneBlock.hpp"
#include "EmptyBlock.hpp"

Blocks::Blocks()
{
	m_blocks["void"] = new EmptyBlock();
	m_blocks["empty"] = new EmptyBlock();
	m_blocks["stone"] = new StoneBlock();
	m_blocks["tnt"] = new TntBlock();
}

Blocks::~Blocks()
{
	m_blocks.clear();
}