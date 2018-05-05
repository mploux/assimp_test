//
// Created by Marc on 11/04/2018.
//

#include "ShaderProgramCreationException.hpp"

ShaderProgramCreationException::ShaderProgramCreationException()
		: ShaderException()
{
	setName("ShaderProgramCreationException");
	setDescription("Unable to create a shader program");
}

ShaderProgramCreationException::~ShaderProgramCreationException() throw() {}