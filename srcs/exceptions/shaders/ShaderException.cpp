//
// Created by Marc on 11/04/2018.
//

#include "ShaderException.hpp"

ShaderException::ShaderException()
		: VoxException()
{
	setName("ShaderException");
	setDescription("Unknown shader exception");
}

ShaderException::~ShaderException() throw() {}