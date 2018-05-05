//
// Created by mploux on 14/04/18.
//

#pragma once

#include <iostream>
#include <map>

#include "ABlock.hpp"

class Blocks
{
private:
	std::map<std::string, ABlock *> m_blocks;

public:
	Blocks();
	virtual ~Blocks();

	ABlock *getBlock(const std::string &name)
	{
		return m_blocks[name];
	}
};
