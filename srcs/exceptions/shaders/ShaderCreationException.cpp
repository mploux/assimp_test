//
// Created by Marc on 11/04/2018.
//

#include "ShaderCreationException.hpp"

ShaderCreationException::ShaderCreationException()
		: ShaderException()
{
	setName("ShaderCreationException");
	setDescription("Unable to create a shader");
}

ShaderCreationException::~ShaderCreationException() throw() {}